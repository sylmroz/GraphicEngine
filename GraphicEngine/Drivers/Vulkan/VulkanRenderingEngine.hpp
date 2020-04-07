#ifndef GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP

#include "../../Common/RenderingEngine.hpp"
#include "../../Common/WindowKeyboardMouse.hpp"
#include "VulkanShader.hpp"
#include "VulkanHelper.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanRenderingEngine : public GraphicEngine::RenderingEngine
	{
	public:
		VulkanRenderingEngine(std::shared_ptr<Common::WindowKeyboardMouse> window, std::shared_ptr<Common::Camera> camera, std::shared_ptr<Core::EventManager> eventManager);
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
		std::unique_ptr<DepthBufferData> _depthBuffer;
		vk::UniqueRenderPass _renderPass;
		std::unique_ptr<ImageData> _image;
		std::vector<vk::UniqueFramebuffer> _frameBuffers;
		std::unique_ptr<RenderingBarriers> _renderingBarriers;
		vk::Queue _graphicQueue;
		vk::Queue _presentQueue;
		vk::UniquePipelineCache _pipelineCache;
		vk::UniquePipelineLayout _pipelineLayout;
		vk::UniquePipeline _graphicPipeline;
		std::unique_ptr<VertexBuffer<Common::VertexPC>> _vertexBuffer;
		std::unique_ptr<VulkanShader> _vertexShader;
		std::unique_ptr<VulkanShader> _fragmentShader;
		std::unique_ptr<UniformBuffer<glm::mat4>> _uniformBuffer;
		
		vk::UniqueDescriptorPool _descriptorPool;
		vk::UniqueDescriptorSetLayout _descriptorSetLayout;
		std::vector<vk::UniqueDescriptorSet> _descriptorSets;
		

	private:
		vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e2;
		uint32_t maxFrames{ 1 };
		uint32_t currentFrameIndex{ 0 };
		QueueFamilyIndices indices;
		bool frameBufferResized{ false };

	private:
		const std::vector<std::string> validationLayers = {
			//"VK_LAYER_LUNARG_standard_validation"
			"VK_LAYER_KHRONOS_validation"
		};
	};
}

#endif // !GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP