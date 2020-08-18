#include "VulkanFramework.hpp"

GraphicEngine::Vulkan::VulkanFramework::VulkanFramework(std::shared_ptr<VulkanWindowContext> vulkanWindowsContext, const std::string& appName, const std::string& engineName, int width, int height, vk::SampleCountFlagBits msaaSamples, const std::vector<std::string>& validationLayers)
{
	initialize(vulkanWindowsContext, appName, engineName, width, height, msaaSamples, validationLayers);
}

GraphicEngine::Vulkan::VulkanFramework& GraphicEngine::Vulkan::VulkanFramework::initialize(std::shared_ptr<VulkanWindowContext> vulkanWindowsContext, const std::string& appName, const std::string& engineName, int width, int height, vk::SampleCountFlagBits msaaSamples, const std::vector<std::string>& validationLayers)
{
	m_vulkanWindowContext = vulkanWindowsContext;
	m_appName = appName;
	m_engineName = engineName;
	m_width = width;
	m_height = height;
	m_msaaSamples = msaaSamples;
	m_validationLayers = validationLayers;

	m_instance = createUniqueInstance(m_appName, m_engineName, m_validationLayers, m_vulkanWindowContext->getRequiredExtensions(), VK_API_VERSION_1_1);
	{
		auto surface = m_vulkanWindowContext->createSurface(m_instance);
		vk::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> _deleter(m_instance.get());
		m_surface = vk::UniqueSurfaceKHR(surface, _deleter);
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
	vk::Extent2D frameBufferSize(width, height);
	this->m_width = width;
	this->m_height = height;
	
	m_device->waitIdle();
	vk::Extent2D frameBufferSize(width, height);
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
