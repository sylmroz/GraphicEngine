#pragma once

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

		VulkanGraphicPipelineInfo(std::shared_ptr<VulkanFramework> framework, vk::UniqueDescriptorSetLayout& descriptorSetLayout, const std::vector<ShaderInfo>& shadersInfo, vk::PrimitiveTopology primitiveTopology,
			bool depthBuffered = true, vk::CullModeFlags cullMode = vk::CullModeFlagBits::eNone, bool depthBoundsTestEnable = false, bool stencilTestEnable = false, vk::CompareOp depthCompareOp = vk::CompareOp::eLess)
		{
			pipelineCache = framework->m_device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
			pipelineLayout = framework->m_device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &descriptorSetLayout.get()));
			graphicPipeline = createGraphicPipeline(framework->m_device, pipelineCache,
				shadersInfo,
				createVertexInputAttributeDescriptions(vertex_type::getSizeAndOffsets()),
				vk::VertexInputBindingDescription(0, vertex_type::getStride()), depthBuffered, vk::FrontFace::eCounterClockwise,
				pipelineLayout, framework->m_renderPass, framework->m_msaaSamples, primitiveTopology, cullMode, depthBoundsTestEnable, stencilTestEnable, depthCompareOp);
		}
	};
}