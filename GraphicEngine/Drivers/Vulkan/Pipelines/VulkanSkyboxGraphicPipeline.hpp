#pragma once

#include "../../../Engines/Graphic/Pipelines/SkyboxGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Shaders/Models/CameraMatrices.hpp"
#include "../VulkanTextureCube.hpp"
#include "VulkanGraphicPipeline.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanSkyboxGraphicPipeline : public Engines::Graphic::SkyboxGraphicPipeline<VulkanSkyboxGraphicPipeline, VertexBuffer, TextureCube, vk::UniqueCommandBuffer&, int>
	{
	public:
		VulkanSkyboxGraphicPipeline(std::shared_ptr<VulkanFramework> framework, std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> cameraUniformBuffer, const std::string& basePath);

		virtual void draw(vk::UniqueCommandBuffer& commandBuffer, int index) override;

		std::unique_ptr<VertexBuffer<Common::VertexP>> produceVertexBuffer(std::vector<Common::VertexP> skyBoxVertices);
		std::shared_ptr<TextureCube> produceTextureCube(std::array<std::string, 6> faces);

	private:
		std::shared_ptr<VulkanFramework> m_framework;
		std::unique_ptr<VulkanGraphicPipelineInfo<Common::VertexP>> m_graphicPipeline;
		vk::UniqueDescriptorPool m_descriptorPool;
		vk::UniqueDescriptorSetLayout m_descriptorSetLayout;
		std::vector<vk::UniqueDescriptorSet> m_descriptorSets;
	};
}