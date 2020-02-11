#ifndef GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP

#include "RenderingEngine.hpp"
#include "../Utils/VulkanHelper.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanRenderingEngine : public GraphicEngine::RenderingEngine
	{
	public:
		VulkanRenderingEngine(std::shared_ptr<Window> window);
		virtual bool drawFrame() override;
		virtual void init(size_t width, size_t height) override;
		virtual void resizeFrameBuffer(size_t width, size_t height) override;
		virtual void cleanup() override;

	private:
		vk::UniqueInstance _instance;
		vk::PhysicalDevice _physicalDevice;
		vk::UniqueSurfaceKHR _surface;
		vk::UniqueDevice _device;

		GraphicEngine::Utils::Vulkan::SwapChainData _swapChainData;
		vk::UniqueCommandPool _commandPool;
		vk::CommandBuffer _commandBuffer;
		std::unique_ptr<GraphicEngine::Utils::Vulkan::DeepBufferData> _depthBuffer;
		vk::UniqueRenderPass _rendePass;
		std::unique_ptr<GraphicEngine::Utils::Vulkan::ImageData> _image;
		std::vector<vk::UniqueFramebuffer> _frameBuffers;

	private:
		vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e4;

	private:
		const std::vector<std::string> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	};
}

#endif // !GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP