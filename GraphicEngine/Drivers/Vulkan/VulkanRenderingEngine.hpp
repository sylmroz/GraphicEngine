#pragma once

#include "../../Common/RenderingEngine.hpp"
#include "VulkanShader.hpp"
#include "VulkanFramework.hpp"
#include "VulkanWindowContext.hpp"
#include "VulkanTexture.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanUniformBuffer.hpp"

#include "../../Engines/Graphic/Shaders/Models/ModelMatrices.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanRenderingEngine : public RenderingEngine
	{
	public:
		VulkanRenderingEngine(std::shared_ptr<VulkanWindowContext> vulkanWindowContext,
			std::shared_ptr<Common::Camera> camera,
			std::shared_ptr<Core::EventManager> eventManager,
			std::shared_ptr<Core::Configuration> cfg,
			std::unique_ptr<Core::Logger<VulkanRenderingEngine>> logger);
		virtual bool drawFrame() override;
		virtual void init(size_t width, size_t height) override;
		virtual void resizeFrameBuffer(size_t width, size_t height) override;
		virtual void cleanup() override;

		virtual ~VulkanRenderingEngine() = default;
	private:
		void buildCommandBuffers();
	private:
		std::unique_ptr<VulkanFramework> m_framework;

		vk::UniquePipelineCache m_pipelineCache;
		vk::UniquePipelineLayout m_pipelineLayout;
		vk::UniquePipeline m_graphicPipeline;
		std::vector<std::vector<std::shared_ptr<VertexBuffer<Common::VertexPN>>>> m_vertexBuffers;
		
		std::shared_ptr<VulkanShader> m_vertexShader;
		std::shared_ptr<VulkanShader> m_fragmentShader;
		std::shared_ptr<UniformBuffer<glm::mat4>> m_uniformBuffer;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::Light>> m_lightUniformBuffer;
		std::shared_ptr<UniformBufferDynamic<Engines::Graphic::Shaders::ModelMartices>> m_modelMatrix;

		vk::UniqueDescriptorPool m_descriptorPool;
		vk::UniqueDescriptorSetLayout m_descriptorSetLayout;
		std::vector<vk::UniqueDescriptorSet> m_descriptorSets;
	private:
		std::shared_ptr<VulkanWindowContext> m_vulkanWindowContext;

	private:
	};
}
