#include "VulkanRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"

#undef max

GraphicEngine::Vulkan::VulkanRenderingEngine::VulkanRenderingEngine(std::shared_ptr<VulkanWindowContext> vulkanWindowContext,
	std::shared_ptr<Common::Camera> camera,
	std::shared_ptr<Core::EventManager> eventManager,
	std::unique_ptr<Core::Logger<VulkanRenderingEngine>> logger) :
	m_vulkanWindowContext(vulkanWindowContext),
	RenderingEngine(camera, eventManager)
{
}

bool GraphicEngine::Vulkan::VulkanRenderingEngine::drawFrame()
{
	try
	{
		m_device->waitForFences(1, &(m_renderingBarriers->inFlightFences[m_currentFrameIndex].get()), true, std::numeric_limits<uint64_t>::max());
		vk::ResultValue<uint32_t> imageIndex = m_device->acquireNextImageKHR(m_swapChainData.swapChain.get(), std::numeric_limits<uint64_t>::max(), m_renderingBarriers->imageAvailableSemaphores[m_currentFrameIndex].get(), vk::Fence());

		if (imageIndex.result == vk::Result::eErrorOutOfDateKHR)
		{
			// Should recreate swapchain
			return false;
		}

		else if (imageIndex.result != vk::Result::eSuccess && imageIndex.result != vk::Result::eSuboptimalKHR)
		{
			throw std::runtime_error("Failed to acquire next image!");
		}

		if (m_renderingBarriers->imagesInFlight[imageIndex.value] != vk::Fence())
			m_device->waitForFences(1, &(m_renderingBarriers->imagesInFlight[imageIndex.value]), true, std::numeric_limits<uint64_t>::max());

		m_renderingBarriers->imagesInFlight[imageIndex.value] = m_renderingBarriers->inFlightFences[m_currentFrameIndex].get();

		vk::Semaphore waitSemaphore(m_renderingBarriers->imageAvailableSemaphores[m_currentFrameIndex].get());
		vk::Semaphore signalSemaphore(m_renderingBarriers->renderFinishedSemaphores[m_currentFrameIndex].get());
		vk::PipelineStageFlags pipelineStageFlags(vk::PipelineStageFlagBits::eColorAttachmentOutput);

		vk::SubmitInfo submitInfo(1, &waitSemaphore, &pipelineStageFlags, 1, &(m_commandBuffers[imageIndex.value].get()), 1, &signalSemaphore);

		m_device->resetFences(1, &(m_renderingBarriers->inFlightFences[m_currentFrameIndex].get()));

		m_uniformBuffer->updateAndSet(m_device, m_camera->getViewProjectionMatrix(), imageIndex.value);

		vk::Result submitResult = m_graphicQueue.submit(1, &submitInfo, m_renderingBarriers->inFlightFences[m_currentFrameIndex].get());
		if (submitResult != vk::Result::eSuccess)
		{
			throw std::runtime_error("Failed to submit draw command buffer!");
		}

		vk::SwapchainKHR sp(m_swapChainData.swapChain.get());

		vk::PresentInfoKHR presentInfo(1, &signalSemaphore, 1, &sp, &imageIndex.value);

		vk::Result presentResult = m_presentQueue.presentKHR(presentInfo);

		if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR)
		{
			// Should recreate swapchain
			return false;
		}
		else if (imageIndex.result != vk::Result::eSuccess)
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

void GraphicEngine::Vulkan::VulkanRenderingEngine::init(size_t width, size_t height)
{
	try
	{
		m_instance = createUniqueInstance("Graphic Engine", "Vulkan Base", m_validationLayers, m_vulkanWindowContext->getRequiredExtensions(), VK_API_VERSION_1_0);
		{
			auto surface = m_vulkanWindowContext->createSurface(m_instance);
			vk::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> _deleter(m_instance.get());
			m_surface = vk::UniqueSurfaceKHR(surface, _deleter);
		}
		m_physicalDevice = getPhysicalDevice(m_instance, m_surface);
		m_device = getUniqueLogicalDevice(m_physicalDevice, m_surface);
		m_indices = findGraphicAndPresentQueueFamilyIndices(m_physicalDevice, m_surface);

		vk::Extent2D frameBufferSize(width, height);
		m_swapChainData = SwapChainData(m_physicalDevice, m_device, m_surface, m_indices, frameBufferSize, vk::UniqueSwapchainKHR(), vk::ImageUsageFlagBits::eColorAttachment);

		m_maxFrames = m_swapChainData.images.size();

		m_commandPool = createUniqueCommandPool(m_device, m_indices);
		m_commandBuffers = m_device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(m_commandPool.get(), vk::CommandBufferLevel::ePrimary, m_maxFrames));

		m_depthBuffer = std::unique_ptr<DepthBufferData>(new DepthBufferData(m_physicalDevice, m_device, vk::Extent3D(frameBufferSize, 1), findDepthFormat(m_physicalDevice), m_msaaSamples));
		m_renderPass = createRenderPass(m_device, m_swapChainData.format, m_depthBuffer->format, m_msaaSamples);
		m_image = std::unique_ptr<ImageData>(new ImageData(m_physicalDevice, m_device,
			vk::Extent3D(m_swapChainData.extent, 1), m_swapChainData.format, m_msaaSamples,
			vk::MemoryPropertyFlagBits::eDeviceLocal, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransientAttachment,
			vk::ImageTiling::eOptimal, 1, vk::ImageLayout::eUndefined, vk::ImageAspectFlagBits::eColor));
		m_frameBuffers = createFrameBuffers(m_device, m_renderPass, m_swapChainData.extent, 1, m_image->imageView, m_depthBuffer->imageView, m_swapChainData.imageViews);

		m_graphicQueue = m_device->getQueue(m_indices.graphicsFamily.value(), 0);
		m_presentQueue = m_device->getQueue(m_indices.presentFamily.value(), 0);

		m_renderingBarriers = std::unique_ptr<RenderingBarriers>(new RenderingBarriers(m_device, m_maxFrames));

		m_vertexBuffer = std::make_unique<VertexBuffer<GraphicEngine::Common::VertexPC>>(m_physicalDevice, m_device, m_commandPool, m_graphicQueue, vertices, RenderingEngine::indices);

		m_vertexShader = std::make_unique<VulkanShader>(m_device, Core::IO::readFile<std::string>("C:/Projects/GraphicEngine/GraphicEngine/Assets/Shaders/Spv/basicPCVP.vert.spv"));
		m_fragmentShader = std::make_unique<VulkanShader>(m_device, Core::IO::readFile<std::string>("C:/Projects/GraphicEngine/GraphicEngine/Assets/Shaders/Spv/basicPCVP.frag.spv"));


		m_uniformBuffer = std::make_unique<UniformBuffer<glm::mat4>>(m_physicalDevice, m_device, m_maxFrames);
		m_descriptorSetLayout = createDescriptorSetLayout(m_device, { {vk::DescriptorType::eUniformBuffer,1,vk::ShaderStageFlagBits::eVertex} }, vk::DescriptorSetLayoutCreateFlags());
		m_descriptorPool = createDescriptorPool(m_device, { vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, m_maxFrames) });
		std::vector<vk::DescriptorSetLayout> layouts(m_maxFrames, m_descriptorSetLayout.get());
		m_descriptorSets = m_device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(m_descriptorPool.get(), m_maxFrames, layouts.data()));
		std::vector<std::vector<std::shared_ptr<BufferData>>> uniformBuffers;
		uniformBuffers.emplace_back(m_uniformBuffer->bufferData);
		updateDescriptorSets(m_device, m_descriptorPool, m_descriptorSetLayout, m_maxFrames, m_descriptorSets, uniformBuffers, {});

		m_pipelineLayout = m_device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &m_descriptorSetLayout.get()));

		m_pipelineCache = m_device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
		m_graphicPipeline = createGraphicPipeline(m_device, m_pipelineCache, ShaderInfo{ m_vertexShader->shaderModule.get(),vk::SpecializationInfo() },
			ShaderInfo{ m_fragmentShader->shaderModule.get(),vk::SpecializationInfo() }, createVertexInputAttributeDescriptions(Common::VertexPC::getSizeAndOffsets()),
			vk::VertexInputBindingDescription(0, Common::VertexPC::getStride()), true, vk::FrontFace::eClockwise, m_pipelineLayout, m_renderPass, m_msaaSamples);

		buildCommandBuffers();
	}

	catch (vk::SystemError& err)
	{
		throw std::runtime_error(err.what());
	}

	catch (std::runtime_error& err)
	{
		throw std::runtime_error(err.what());
	}

	catch (...)
	{
		throw std::runtime_error("Vulkan Initialize: unknown error\n");
	}
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::resizeFrameBuffer(size_t width, size_t height)
{
	if (width == 0 || height == 0)
		return;

	m_device->waitIdle();
	vk::Extent2D frameBufferSize(width, height);
	m_swapChainData = SwapChainData(m_physicalDevice, m_device, m_surface, m_indices, frameBufferSize, m_swapChainData.swapChain, vk::ImageUsageFlagBits::eColorAttachment);
	m_commandBuffers = m_device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(m_commandPool.get(), vk::CommandBufferLevel::ePrimary, m_maxFrames));

	m_depthBuffer = std::unique_ptr<DepthBufferData>(new DepthBufferData(m_physicalDevice, m_device, vk::Extent3D(frameBufferSize, 1), findDepthFormat(m_physicalDevice), m_msaaSamples));
	m_renderPass = createRenderPass(m_device, m_swapChainData.format, m_depthBuffer->format, m_msaaSamples);
	m_image = std::unique_ptr<ImageData>(new ImageData(m_physicalDevice, m_device,
		vk::Extent3D(m_swapChainData.extent, 1), m_swapChainData.format, m_msaaSamples,
		vk::MemoryPropertyFlagBits::eDeviceLocal, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransientAttachment,
		vk::ImageTiling::eOptimal, 1, vk::ImageLayout::eUndefined, vk::ImageAspectFlagBits::eColor));
	m_frameBuffers = createFrameBuffers(m_device, m_renderPass, m_swapChainData.extent, 1, m_image->imageView, m_depthBuffer->imageView, m_swapChainData.imageViews);

	/*m_uniformBuffer = std::make_unique<UniformBuffer<glm::mat4>>(m_physicalDevice, m_device, m_maxFrames);
	m_descriptorSetLayout = createDescriptorSetLayout(m_device, { {vk::DescriptorType::eUniformBuffer,1,vk::ShaderStageFlagBits::eVertex} }, vk::DescriptorSetLayoutCreateFlags());
	m_descriptorPool = createDescriptorPool(m_device, { vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, m_maxFrames) });
	std::vector<vk::DescriptorSetLayout> layouts(m_maxFrames, m_descriptorSetLayout.get());
	m_descriptorSets = m_device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(m_descriptorPool.get(), m_maxFrames, layouts.data()));
	std::vector<std::vector<std::shared_ptr<BufferData>>> uniformBuffers;
	uniformBuffers.emplace_back(m_uniformBuffer->bufferData);
	updateDescriptorSets(m_device, m_descriptorPool, m_descriptorSetLayout, m_maxFrames, m_descriptorSets, uniformBuffers, {});

	m_pipelineLayout = m_device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &m_descriptorSetLayout.get()));

	m_pipelineCache = m_device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
	m_graphicPipeline = createGraphicPipeline(m_device, m_pipelineCache, ShaderInfo{ m_vertexShader->shaderModule.get(),vk::SpecializationInfo() },
		ShaderInfo{ m_fragmentShader->shaderModule.get(),vk::SpecializationInfo() }, createVertexInputAttributeDescriptions(Common::VertexPC::getSizeAndOffsets()),
		vk::VertexInputBindingDescription(0, Common::VertexPC::getStride()), true, vk::FrontFace::eClockwise, m_pipelineLayout, m_renderPass, m_msaaSamples);*/

	buildCommandBuffers();
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::cleanup()
{
	m_graphicQueue.waitIdle();
	m_device->waitIdle();
}

uint32_t GraphicEngine::Vulkan::VulkanRenderingEngine::calculateNextIndex()
{
	return (m_currentFrameIndex + 1) % m_maxFrames;
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::buildCommandBuffers()
{
	std::array<vk::ClearValue, 3> clearValues;

	clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f,0.2f,0.2f,1.0f }));
	clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0.0f);
	clearValues[2].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f,0.2f,0.2f,1.0f }));
	int i{ 0 };
	for (auto& commandBuffer : m_commandBuffers)
	{
		commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlags()));

		commandBuffer->setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(m_swapChainData.extent.width), static_cast<float>(m_swapChainData.extent.height), 0.0f, 1.0f));
		commandBuffer->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_swapChainData.extent));

		vk::RenderPassBeginInfo renderPassBeginInfo(m_renderPass.get(), m_frameBuffers[i].get(), vk::Rect2D(vk::Offset2D(0, 0), m_swapChainData.extent), static_cast<uint32_t>(clearValues.size()), clearValues.data());
		commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

		commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicPipeline.get());

		commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout.get(), 0, 1, &m_descriptorSets[i].get(), 0, nullptr);

		m_vertexBuffer->bind(commandBuffer);
		m_vertexBuffer->draw(commandBuffer);

		commandBuffer->endRenderPass();

		commandBuffer->end();
		++i;
	}
}
