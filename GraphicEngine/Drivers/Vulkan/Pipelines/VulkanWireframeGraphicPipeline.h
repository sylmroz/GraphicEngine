#pragma once

#include "../../../Engines/Graphic/Pipelines/WireframeGraphicPipeline.hpp"
#include "../VulkanFramework.hpp"
#include "../VulkanUniformBuffer.hpp"
#include "../VulkanVertexBuffer.hpp"

namespace GraphicEngine::Vulkan
{
	template <typename VertexType>
	struct VulkanGraphicPipelineInfo
	{
		using vertex_type = VertexType;
		vk::UniquePipeline graphicPipeline;
		vk::UniquePipelineCache pipelineCache;
		vk::UniquePipelineLayout pipelineLayout;

		VulkanGraphicPipelineInfo(std::shared_ptr<VulkanFramework> framework, vk::UniqueDescriptorSetLayout& descriptorSetLayout, const std::vector<ShaderInfo>& shadersInfo, vk::PrimitiveTopology primitiveTopology)
		{
			pipelineCache = framework->m_device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
			pipelineLayout = framework->m_device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &descriptorSetLayout.get()));
			graphicPipeline = createGraphicPipeline(framework->m_device, pipelineCache,
				shadersInfo,
				createVertexInputAttributeDescriptions(vertex_type::getSizeAndOffsets()),
				vk::VertexInputBindingDescription(0, vertex_type::getStride()), true, vk::FrontFace::eClockwise, 
				pipelineLayout, framework->m_renderPass, framework->m_msaaSamples, primitiveTopology);
		}
	};

	class VulkanWireframeGraphicPipeline : public Engines::Graphic::WireframeGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>
	{
	public:
		VulkanWireframeGraphicPipeline(std::shared_ptr<VulkanFramework> framework, std::shared_ptr<UniformBuffer<glm::mat4>> cameraUniformBuffer, std::shared_ptr<Services::CameraControllerManager> cameraControllerManager);

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
