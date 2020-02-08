#include "VulkanRenderingEngine.hpp"
#include "../Window/WindowGLFW.hpp"

#include <iostream>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

GraphicEngine::Vulkan::VulkanRenderingEngine::VulkanRenderingEngine(std::shared_ptr<Window> window):
	RenderingEngine(window)
{
}

bool GraphicEngine::Vulkan::VulkanRenderingEngine::drawFrame()
{
	return false;
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
		QueueFamilyIndices indices = findGraphicAndPresentQueueFamilyIndices(_physicalDevice, _surface);
		_commandPool = createUniqueCommandPool(_device, indices);
		auto [w, h] = _window->getFrameBufferSize();
		vk::Extent2D frameBufferSize(w, h);
		_swapChainData = SwapChainData(_physicalDevice, _device, _surface, indices, frameBufferSize, vk::UniqueSwapchainKHR(), vk::ImageUsageFlagBits::eColorAttachment);
		_deepBuffer = std::unique_ptr<DeepBufferData>(new DeepBufferData(_physicalDevice, _device, vk::Extent3D(frameBufferSize, 1), /*findSupportedFormats*/ vk::Format::eD16Unorm,/*selactMsaa*/ vk::SampleCountFlagBits::e1));
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
