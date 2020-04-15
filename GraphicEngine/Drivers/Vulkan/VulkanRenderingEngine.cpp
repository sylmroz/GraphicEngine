#include "VulkanRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"

#undef max

GraphicEngine::Vulkan::VulkanRenderingEngine::VulkanRenderingEngine(std::shared_ptr<VulkanWindowContext> vulkanWindowContext,
	std::shared_ptr<Common::Camera> camera, std::shared_ptr<Core::EventManager> eventManager) :
	_vulkanWindowContext(vulkanWindowContext),
	RenderingEngine(camera, eventManager)
{
}

bool GraphicEngine::Vulkan::VulkanRenderingEngine::drawFrame()
{
	_device->waitForFences(1, &(_renderingBarriers->inFlightFences[currentFrameIndex].get()), true, std::numeric_limits<uint64_t>::max());
	vk::ResultValue<uint32_t> imageIndex = _device->acquireNextImageKHR(_swapChainData.swapChain.get(), std::numeric_limits<uint64_t>::max(), _renderingBarriers->imageAvailableSemaphores[currentFrameIndex].get(), vk::Fence());

	if (imageIndex.result == vk::Result::eErrorOutOfDateKHR)
	{
		// Should recreate swapchain
		return false;
	}

	else if (imageIndex.result != vk::Result::eSuccess && imageIndex.result != vk::Result::eSuboptimalKHR)
	{
		throw std::runtime_error("Failed to acquire next image!");
	}

	if (_renderingBarriers->imagesInFlight[imageIndex.value] != vk::Fence())
		_device->waitForFences(1, &(_renderingBarriers->imagesInFlight[imageIndex.value]), true, std::numeric_limits<uint64_t>::max());

	_renderingBarriers->imagesInFlight[imageIndex.value] = _renderingBarriers->inFlightFences[currentFrameIndex].get();

	vk::Semaphore waitSemaphore(_renderingBarriers->imageAvailableSemaphores[currentFrameIndex].get());
	vk::Semaphore signalSemaphore(_renderingBarriers->renderFinishedSemaphores[currentFrameIndex].get());
	vk::PipelineStageFlags pipelineStageFlags(vk::PipelineStageFlagBits::eColorAttachmentOutput);

	vk::SubmitInfo submitInfo(1, &waitSemaphore, &pipelineStageFlags, 1, &(_commandBuffers[imageIndex.value].get()), 1, &signalSemaphore);

	_device->resetFences(1, &(_renderingBarriers->inFlightFences[currentFrameIndex].get()));

	_uniformBuffer->updateAndSet(_device, _camera->getViewProjectionMatrix(), imageIndex.value);
	
	vk::Result submitResult = _graphicQueue.submit(1, &submitInfo, _renderingBarriers->inFlightFences[currentFrameIndex].get());
	if (submitResult != vk::Result::eSuccess)
	{
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	vk::SwapchainKHR sp(_swapChainData.swapChain.get());

	vk::PresentInfoKHR presentInfo(1, &signalSemaphore, 1, &sp, &imageIndex.value);
	vk::Result presentResult = _presentQueue.presentKHR(presentInfo);

	if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || frameBufferResized)
	{
		frameBufferResized = false;
		// Should recreate swapchain
	}

	currentFrameIndex = calculateNextIndex();
	return true;
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::init(size_t width, size_t height)
{
	try
	{
		_instance = createUniqueInstance("Graphic Engine", "Vulkan Base", validationLayers, _vulkanWindowContext->getRequiredExtensions(), VK_API_VERSION_1_0);
		{
			auto surface = _vulkanWindowContext->createSurface(_instance);
			vk::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> _deleter(_instance.get());
			_surface = vk::UniqueSurfaceKHR(surface, _deleter);
		}
		_physicalDevice = getPhysicalDevice(_instance, _surface);
		_device = getUniqueLogicalDevice(_physicalDevice, _surface);
		indices = findGraphicAndPresentQueueFamilyIndices(_physicalDevice, _surface);

		vk::Extent2D frameBufferSize(width, height);
		_swapChainData = SwapChainData(_physicalDevice, _device, _surface, indices, frameBufferSize, vk::UniqueSwapchainKHR(), vk::ImageUsageFlagBits::eColorAttachment);

		maxFrames = _swapChainData.images.size();

		_commandPool = createUniqueCommandPool(_device, indices);
		_commandBuffers = _device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(_commandPool.get(), vk::CommandBufferLevel::ePrimary, maxFrames));

		_depthBuffer = std::unique_ptr<DepthBufferData>(new DepthBufferData(_physicalDevice, _device, vk::Extent3D(frameBufferSize, 1), findDepthFormat(_physicalDevice), msaaSamples));
		_renderPass = createRenderPass(_device, _swapChainData.format, _depthBuffer->format, msaaSamples);
		_image = std::unique_ptr<ImageData>(new ImageData(_physicalDevice, _device,
			vk::Extent3D(_swapChainData.extent, 1), _swapChainData.format, msaaSamples,
			vk::MemoryPropertyFlagBits::eDeviceLocal, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransientAttachment,
			vk::ImageTiling::eOptimal, 1, vk::ImageLayout::eUndefined, vk::ImageAspectFlagBits::eColor));
		_frameBuffers = createFrameBuffers(_device, _renderPass, _swapChainData.extent, 1, _image->imageView, _depthBuffer->imageView, _swapChainData.imageViews);

		_graphicQueue = _device->getQueue(indices.graphicsFamily.value(), 0);
		_presentQueue = _device->getQueue(indices.presentFamily.value(), 0);

		_renderingBarriers = std::unique_ptr<RenderingBarriers>(new RenderingBarriers(_device, maxFrames));

		_vertexBuffer = std::make_unique<VertexBuffer<GraphicEngine::Common::VertexPC>>(_physicalDevice, _device, _commandPool, _graphicQueue, vertices, RenderingEngine::indices);

		_vertexShader = std::make_unique<VulkanShader>(_device, Core::IO::readFile<std::string>("C:/Projects/GraphicEngine/GraphicEngine/Assets/Shaders/Spv/basicPCVP.vert.spv"));
		_fragmentShader = std::make_unique<VulkanShader>(_device, Core::IO::readFile<std::string>("C:/Projects/GraphicEngine/GraphicEngine/Assets/Shaders/Spv/basicPCVP.frag.spv"));

		
		_uniformBuffer = std::make_unique<UniformBuffer<glm::mat4>>(_physicalDevice, _device, maxFrames);
		_descriptorSetLayout = createDescriptorSetLayout(_device, { {vk::DescriptorType::eUniformBuffer,1,vk::ShaderStageFlagBits::eVertex} }, vk::DescriptorSetLayoutCreateFlags());
		_descriptorPool = createDescriptorPool(_device, { vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, maxFrames) });
		std::vector<vk::DescriptorSetLayout> layouts(maxFrames, _descriptorSetLayout.get());
		_descriptorSets = _device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(_descriptorPool.get(), maxFrames, layouts.data()));
		std::vector<std::vector<std::shared_ptr<BufferData>>> uniformBuffers;
		uniformBuffers.emplace_back(_uniformBuffer->_bufferData);
		updateDescriptorSets(_device, _descriptorPool, _descriptorSetLayout, maxFrames, _descriptorSets, uniformBuffers, {});
		
		_pipelineLayout = _device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &_descriptorSetLayout.get()));

		_pipelineCache = _device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
		_graphicPipeline = createGraphicPipeline(_device, _pipelineCache, ShaderInfo{ _vertexShader->shaderModule.get(),vk::SpecializationInfo() },
			ShaderInfo{ _fragmentShader->shaderModule.get(),vk::SpecializationInfo() }, createVertexInputAttributeDescriptions(Common::VertexPC::getSizeAndOffsets()),
			vk::VertexInputBindingDescription(0, Common::VertexPC::getStride()), true, vk::FrontFace::eClockwise, _pipelineLayout, _renderPass, msaaSamples);
		
		

		buildCommandBuffers();
	}

	catch (vk::SystemError & err)
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
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::cleanup()
{
	_graphicQueue.waitIdle();
	_device->waitIdle();
}

uint32_t GraphicEngine::Vulkan::VulkanRenderingEngine::calculateNextIndex()
{
	return (currentFrameIndex + 1) % maxFrames;
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::buildCommandBuffers()
{
	std::array<vk::ClearValue, 3> clearValues;

	clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f,0.2f,0.2f,1.0f }));
	clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0.0f);
	clearValues[2].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f,0.2f,0.2f,1.0f }));
	int i{ 0 };
	for (auto& commandBuffer : _commandBuffers)
	{
		commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlags()));
		
		commandBuffer->setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(_swapChainData.extent.width), static_cast<float>(_swapChainData.extent.height), 0.0f, 1.0f));
		commandBuffer->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), _swapChainData.extent));

		vk::RenderPassBeginInfo renderPassBeginInfo(_renderPass.get(), _frameBuffers[i].get(), vk::Rect2D(vk::Offset2D(0, 0), _swapChainData.extent), static_cast<uint32_t>(clearValues.size()), clearValues.data());
		commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

		commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, _graphicPipeline.get());

		commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipelineLayout.get(), 0, 1, &_descriptorSets[i].get(), 0, nullptr);
		
		_vertexBuffer->bind(commandBuffer);
		_vertexBuffer->draw(commandBuffer);

		commandBuffer->endRenderPass();

		commandBuffer->end();
		++i;
	}
}
