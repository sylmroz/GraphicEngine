#pragma once

#include "../../Common/RenderingEngine.hpp"
#include "VulkanShader.hpp"
#include "VulkanHelper.hpp"
#include "VulkanWindowContext.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanRenderingEngine : public RenderingEngine
	{
	public:
		VulkanRenderingEngine(std::shared_ptr<VulkanWindowContext> vulkanWindowContext,
			std::shared_ptr<Common::Camera> camera,
			std::shared_ptr<Core::EventManager> eventManager,
			std::unique_ptr<Core::Logger<VulkanRenderingEngine>> logger);
		virtual bool drawFrame() override;
		virtual void init(size_t width, size_t height) override;
		virtual void resizeFrameBuffer(size_t width, size_t height) override;
		virtual void cleanup() override;

		virtual ~VulkanRenderingEngine() = default;
	private:
		uint32_t calculateNextIndex();
		void buildCommandBuffers();
	private:
		vk::UniqueInstance m_instance;
		vk::PhysicalDevice m_physicalDevice;
		vk::UniqueSurfaceKHR m_surface;
		vk::UniqueDevice m_device;

		SwapChainData m_swapChainData;
		vk::UniqueCommandPool m_commandPool;
		std::vector<vk::UniqueCommandBuffer> m_commandBuffers;
		std::unique_ptr<DepthBufferData> m_depthBuffer;
		vk::UniqueRenderPass m_renderPass;
		std::unique_ptr<ImageData> m_image;
		std::vector<vk::UniqueFramebuffer> m_frameBuffers;
		std::unique_ptr<RenderingBarriers> m_renderingBarriers;
		vk::Queue m_graphicQueue;
		vk::Queue m_presentQueue;
		vk::UniquePipelineCache m_pipelineCache;
		vk::UniquePipelineLayout m_pipelineLayout;
		vk::UniquePipeline m_graphicPipeline;
		std::unique_ptr<VertexBuffer<Common::VertexPC>> m_vertexBuffer;
		std::unique_ptr<VulkanShader> m_vertexShader;
		std::unique_ptr<VulkanShader> m_fragmentShader;
		std::unique_ptr<UniformBuffer<glm::mat4>> m_uniformBuffer;

		vk::UniqueDescriptorPool m_descriptorPool;
		vk::UniqueDescriptorSetLayout m_descriptorSetLayout;
		std::vector<vk::UniqueDescriptorSet> m_descriptorSets;


	private:
		vk::SampleCountFlagBits m_msaaSamples = vk::SampleCountFlagBits::e2;
		uint32_t m_maxFrames{ 1 };
		uint32_t m_currentFrameIndex{ 0 };
		QueueFamilyIndices m_indices;

		std::shared_ptr<VulkanWindowContext> m_vulkanWindowContext;

	private:
		const std::vector<std::string> m_validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	};
}
