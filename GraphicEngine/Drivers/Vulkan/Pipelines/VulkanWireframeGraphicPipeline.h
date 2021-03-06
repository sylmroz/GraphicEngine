#pragma once

#include "../../../Engines/Graphic/Pipelines/WireframeGraphicPipeline.hpp"
#include "VulkanGraphicPipeline.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanWireframeGraphicPipeline : public Engines::Graphic::WireframeGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>
	{
	public:
		VulkanWireframeGraphicPipeline(std::shared_ptr<VulkanFramework> framework, std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> cameraUniformBuffer);

		template <typename VertexType>
		void addVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			Engines::Graphic::WireframeGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>::addVertexBuffer(mesh, vertexBuffer);
			addUniformBuffer();
		}

		template <typename VertexType>
		void eraseVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh)
		{
			Engines::Graphic::WireframeGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>::eraseVertexBuffer(mesh);
			deleteUniformBuffer();
		}

		template <typename VertexType>
		void eraseVertexBuffer(std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			Engines::Graphic::WireframeGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>::eraseVertexBuffer(vertexBuffer);
			deleteUniformBuffer();
		}

		virtual void draw(vk::UniqueCommandBuffer& commandBuffer, int index) override;

		void updateDynamicUniforms();

	private:
		void addUniformBuffer();
		void deleteUniformBuffer();

	private:
		std::shared_ptr<VulkanFramework> m_framework;
		std::shared_ptr<Common::EntityByVertexTypeManager<VulkanGraphicPipelineInfo>> m_vulkanGraphicPipelines;
		uint32_t alignedSize;
		std::vector<Engines::Graphic::Shaders::WireframeModelDescriptor> m_wireframeModelDescriptors;

	private:
		vk::UniqueDescriptorPool m_descriptorPool;
		vk::UniqueDescriptorSetLayout m_descriptorSetLayout;
		std::vector<vk::UniqueDescriptorSet> m_descriptorSets;
	};
}
