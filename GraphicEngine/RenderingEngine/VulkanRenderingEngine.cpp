#include "VulkanRenderingEngine.hpp"
#include "../Window/WindowGLFW.hpp"

#include <iostream>

#undef max

GraphicEngine::Vulkan::VulkanRenderingEngine::VulkanRenderingEngine(std::shared_ptr<Window> window):
	RenderingEngine(window)
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

	if (_renderingBarriers->imagesInFlight[imageIndex.value].get()!=VK_NULL_HANDLE)
		_device->waitForFences(1, &(_renderingBarriers->inFlightFences[imageIndex.value].get()), true, std::numeric_limits<uint64_t>::max());

	_renderingBarriers->imagesInFlight[imageIndex.value].get() = _renderingBarriers->inFlightFences[currentFrameIndex].get();
	
	_device->resetFences(1, &(_renderingBarriers->inFlightFences[currentFrameIndex].get()));

	vk::Semaphore waitSemaphore(_renderingBarriers->imageAvailableSemaphores[currentFrameIndex].get());
	vk::Semaphore signalSemaphore(_renderingBarriers->renderFinishedSemaphores[currentFrameIndex].get());
	vk::PipelineStageFlags pipelineStageFlags(vk::PipelineStageFlagBits::eColorAttachmentOutput);

	vk::SubmitInfo submitInfo(1, &waitSemaphore, &pipelineStageFlags, 1, nullptr /*Will be command buffer*/, 1, &signalSemaphore);

	vk::Result submitResult = _graphicQueue.submit(1, &submitInfo, _renderingBarriers->inFlightFences[currentFrameIndex].get());
	if (submitResult != vk::Result::eSuccess)
	{
		throw std::runtime_error("Failed to submit draw command buffer!");
	}
	
	vk::SwapchainKHR sp(_swapChainData.swapChain.get());

	vk::PresentInfoKHR presentInfo(1, &waitSemaphore, 1, &sp, &imageIndex.value);
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
	using namespace GraphicEngine::Utils::Vulkan;
	try
	{
		_instance = createUniqueInstance("Graphic Engine", "Vulklan Base", validationLayers, _window->getRequiredExtensions(), VK_API_VERSION_1_0);
		{
			auto surface = std::dynamic_pointer_cast<GraphicEngine::GLFW::WindowGLFW>(_window)->getWindowSurface(_instance);
			vk::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> _deleter(_instance.get());
			_surface = vk::UniqueSurfaceKHR(surface, _deleter);
		}
		_physicalDevice = getPhysicalDevice(_instance, _surface);
		_device = getUniqueLogicalDevice(_physicalDevice, _surface);
		indices = findGraphicAndPresentQueueFamilyIndices(_physicalDevice, _surface);

		_commandPool = createUniqueCommandPool(_device, indices);
		auto [w, h] = _window->getFrameBufferSize();
		vk::Extent2D frameBufferSize(w, h);
		_swapChainData = SwapChainData(_physicalDevice, _device, _surface, indices, frameBufferSize, vk::UniqueSwapchainKHR(), vk::ImageUsageFlagBits::eColorAttachment);
		maxFrames = _swapChainData.images.size();
		_renderingBarriers = std::unique_ptr<RenderingBarriers>(new RenderingBarriers(_device, maxFrames));
		_depthBuffer = std::unique_ptr<DeepBufferData>(new DeepBufferData(_physicalDevice, _device, vk::Extent3D(frameBufferSize, 1), findDepthFormat(_physicalDevice), msaaSamples));
		_rendePass = createRenderPass(_device, _swapChainData.format, _depthBuffer->format, msaaSamples);
		_image = std::unique_ptr<ImageData>(new ImageData(_physicalDevice, _device,
			vk::Extent3D(_swapChainData.extent, 1), _swapChainData.format, msaaSamples,
			vk::MemoryPropertyFlagBits::eDeviceLocal, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransientAttachment,
			vk::ImageTiling::eOptimal, 1, vk::ImageLayout::eUndefined, vk::ImageAspectFlagBits::eColor));
		_frameBuffers = createFrameBuffers(_device, _rendePass, _swapChainData.extent, 1, _image->imageView, _depthBuffer->imageView, _swapChainData.imageViews);
		_graphicQueue = _device->getQueue(indices.graphicsFamily.value(), 0);
		_presentQueue = _device->getQueue(indices.presentFamily.value(), 0);
	}

	catch (vk::SystemError & err)
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
}

uint32_t GraphicEngine::Vulkan::VulkanRenderingEngine::calculateNextIndex()
{
	return (currentFrameIndex + 1) % maxFrames;
}
