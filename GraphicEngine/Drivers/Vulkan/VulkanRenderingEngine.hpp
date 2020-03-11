#ifndef GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP

#include "../../Common/RenderingEngine.hpp"
#include "VulkanShader.hpp"
#include "VulkanHelper.hpp"

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
		uint32_t calculateNextIndex();
		void buildCommandBuffers();
	private:
		vk::UniqueInstance _instance;
		vk::PhysicalDevice _physicalDevice;
		vk::UniqueSurfaceKHR _surface;
		vk::UniqueDevice _device;

		SwapChainData _swapChainData;
		vk::UniqueCommandPool _commandPool;
		std::vector<vk::UniqueCommandBuffer> _commandBuffers;
		std::unique_ptr<DeepBufferData> _depthBuffer;
		vk::UniqueRenderPass _rendePass;
		std::unique_ptr<ImageData> _image;
		std::vector<vk::UniqueFramebuffer> _frameBuffers;
		std::unique_ptr<RenderingBarriers> _renderingBarriers;
		vk::Queue _graphicQueue;
		vk::Queue _presentQueue;

	private:
		vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e2;
		uint32_t maxFrames{ 1 };
		uint32_t currentFrameIndex{ 0 };
		QueueFamilyIndices indices;
		bool frameBufferResized{ false };

	private:
		const std::vector<std::string> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	};
}

#endif // !GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP