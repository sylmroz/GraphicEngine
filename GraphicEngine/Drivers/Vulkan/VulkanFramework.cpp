#include "VulkanFramework.hpp"

#undef max

GraphicEngine::Vulkan::VulkanFramework::VulkanFramework():
	VulkanShaderFactory{ this }
{
}

GraphicEngine::Vulkan::VulkanFramework::VulkanFramework(std::shared_ptr<VulkanWindowContext> vulkanWindowsContext, const std::string& appName, const std::string& engineName, int width, int height, vk::SampleCountFlagBits msaaSamples, const std::vector<std::string>& validationLayers, std::unique_ptr<GraphicEngine::Core::Logger<VulkanFramework>> logger):
	VulkanShaderFactory{ this }
{
	initialize(vulkanWindowsContext, appName, engineName, width, height, msaaSamples, validationLayers, std::move(logger));
}

GraphicEngine::Vulkan::VulkanFramework& GraphicEngine::Vulkan::VulkanFramework::initialize(std::shared_ptr<VulkanWindowContext> vulkanWindowsContext, const std::string& appName, const std::string& engineName, int width, int height, vk::SampleCountFlagBits msaaSamples, const std::vector<std::string>& validationLayers, std::unique_ptr<Core::Logger<VulkanFramework>> logger)
{
	m_vulkanWindowContext = vulkanWindowsContext;
	m_appName = appName;
	m_engineName = engineName;
	m_width = width;
	m_height = height;
	m_msaaSamples = msaaSamples;
	m_validationLayers = validationLayers;
	m_logger = std::move(logger);
	m_logger->debug(__FILE__, __LINE__, __FUNCTION__, "Initialize Vulkan Framework");

	m_instance = createUniqueInstance(m_appName, m_engineName, m_validationLayers, m_vulkanWindowContext->getRequiredExtensions(), VK_API_VERSION_1_1);
	{
		auto surface = m_vulkanWindowContext->createSurface(m_instance);
		vk::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> _deleter(m_instance.get());
		m_surface = vk::UniqueSurfaceKHR(surface, _deleter);
		m_logger->debug(__FILE__, __LINE__, __FUNCTION__, "Create Surface Instance");
	}
	m_physicalDevice = getPhysicalDevice(m_instance, m_surface);
	m_device = getUniqueLogicalDevice(m_physicalDevice, m_surface);
	m_indices = findGraphicAndPresentQueueFamilyIndices(m_physicalDevice, m_surface);

	m_graphicQueue = m_device->getQueue(m_indices.graphicsFamily.value(), 0);
	m_presentQueue = m_device->getQueue(m_indices.presentFamily.value(), 0);

	return *this;
}

GraphicEngine::Vulkan::VulkanFramework& GraphicEngine::Vulkan::VulkanFramework::initializeFramebuffer(int width, int height)
{
	m_logger->debug(__FILE__, __LINE__, __FUNCTION__, "Initialize frame buffer. Width {}, Height {}", width, height);
	vk::Extent2D frameBufferSize(width, height);
	this->m_width = width;
	this->m_height = height;
	
	m_device->waitIdle();
	m_swapChainData = SwapChainData(m_physicalDevice, m_device, m_surface, m_indices, frameBufferSize, m_swapChainData.swapChain, vk::ImageUsageFlagBits::eColorAttachment);
	m_maxFrames = m_swapChainData.images.size();

	m_commandBuffers = m_device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(m_commandPool.get(), vk::CommandBufferLevel::ePrimary, m_maxFrames));

	m_depthBuffer = std::make_unique<DepthBufferData>(m_physicalDevice, m_device, vk::Extent3D(frameBufferSize, 1), findDepthFormat(m_physicalDevice), m_msaaSamples);
	m_renderPass = createRenderPass(m_device, m_swapChainData.format, m_depthBuffer->format, m_msaaSamples);
	m_image = std::make_unique<ImageData>(m_physicalDevice, m_device,
		vk::Extent3D(m_swapChainData.extent, 1), m_swapChainData.format, m_msaaSamples,
		vk::MemoryPropertyFlagBits::eDeviceLocal, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransientAttachment,
		vk::ImageTiling::eOptimal, 1, vk::ImageLayout::eUndefined, vk::ImageAspectFlagBits::eColor);
	m_frameBuffers = createFrameBuffers(m_device, m_renderPass, m_swapChainData.extent, 1, m_image->imageView, m_depthBuffer->imageView, m_swapChainData.imageViews);

	return *this;
}

GraphicEngine::Vulkan::VulkanFramework& GraphicEngine::Vulkan::VulkanFramework::initializeFramebuffer()
{
	return this->initializeFramebuffer(m_width, m_height);
}

GraphicEngine::Vulkan::VulkanFramework& GraphicEngine::Vulkan::VulkanFramework::initializeCommandBuffer()
{
	m_commandPool = createUniqueCommandPool(m_device, m_indices);
	m_commandBuffers = m_device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(m_commandPool.get(), vk::CommandBufferLevel::ePrimary, m_maxFrames));

	return *this;
}

GraphicEngine::Vulkan::VulkanFramework& GraphicEngine::Vulkan::VulkanFramework::initalizeRenderingBarriers()
{
	m_renderingBarriers = std::make_unique<RenderingBarriers>(m_device, m_maxFrames);

	return *this;
}

bool GraphicEngine::Vulkan::VulkanFramework::acquireFrame()
{
	try
	{
		m_device->waitForFences(1, &(m_renderingBarriers->inFlightFences[m_currentFrameIndex].get()), true, std::numeric_limits<uint64_t>::max());
		m_imageIndex = m_device->acquireNextImageKHR(m_swapChainData.swapChain.get(), std::numeric_limits<uint64_t>::max(), m_renderingBarriers->imageAvailableSemaphores[m_currentFrameIndex].get(), vk::Fence());

		if (m_imageIndex.result == vk::Result::eErrorOutOfDateKHR)
		{
			// Should recreate swapchain
			return false;
		}

		else if (m_imageIndex.result != vk::Result::eSuccess && m_imageIndex.result != vk::Result::eSuboptimalKHR)
		{
			throw std::runtime_error("Failed to acquire next image!");
		}

		if (m_renderingBarriers->imagesInFlight[m_imageIndex.value] != vk::Fence())
			m_device->waitForFences(1, &(m_renderingBarriers->imagesInFlight[m_imageIndex.value]), true, std::numeric_limits<uint64_t>::max());

		m_renderingBarriers->imagesInFlight[m_imageIndex.value] = m_renderingBarriers->inFlightFences[m_currentFrameIndex].get();

		m_device->resetFences(1, &(m_renderingBarriers->inFlightFences[m_currentFrameIndex].get()));
	}
	catch (vk::OutOfDateKHRError err)
	{
		// Should recreate swapchain
		return false;
	}
	return true;
}

bool GraphicEngine::Vulkan::VulkanFramework::submitFrame()
{
	try
	{
		vk::Semaphore waitSemaphore(m_renderingBarriers->imageAvailableSemaphores[m_currentFrameIndex].get());
		vk::Semaphore signalSemaphore(m_renderingBarriers->renderFinishedSemaphores[m_currentFrameIndex].get());
		vk::PipelineStageFlags pipelineStageFlags(vk::PipelineStageFlagBits::eColorAttachmentOutput);
		vk::SubmitInfo submitInfo(1, &waitSemaphore, &pipelineStageFlags, 1, &(m_commandBuffers[m_imageIndex.value].get()), 1, &signalSemaphore);
		vk::Result submitResult = m_graphicQueue.submit(1, &submitInfo, m_renderingBarriers->inFlightFences[m_currentFrameIndex].get());
		if (submitResult != vk::Result::eSuccess)
		{
			throw std::runtime_error("Failed to submit draw command buffer!");
		}

		vk::SwapchainKHR sp(m_swapChainData.swapChain.get());

		vk::PresentInfoKHR presentInfo(1, &signalSemaphore, 1, &sp, &m_imageIndex.value);

		vk::Result presentResult = m_presentQueue.presentKHR(presentInfo);

		if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR)
		{
			// Should recreate swapchain
			return false;
		}
		else if (m_imageIndex.result != vk::Result::eSuccess)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}
	}

	catch (vk::OutOfDateKHRError err)
	{
		// Should recreate swapchain
		return false;
	}

	m_currentFrameIndex = calculateNextIndex();
	return true;
}

uint32_t GraphicEngine::Vulkan::VulkanFramework::calculateNextIndex()
{
	return (m_currentFrameIndex + 1) % m_maxFrames;
}