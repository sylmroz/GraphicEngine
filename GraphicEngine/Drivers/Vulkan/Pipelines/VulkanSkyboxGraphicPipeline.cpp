#include "VulkanSkyboxGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::Vulkan::VulkanSkyboxGraphicPipeline::VulkanSkyboxGraphicPipeline(std::shared_ptr<VulkanFramework> framework, std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> cameraUniformBuffer, const std::string& basePath)
{
	m_framework = framework;
	initialize(basePath);

	m_descriptorSetLayout = createDescriptorSetLayout(m_framework->m_device,
		{ {cameraUniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eVertex},
		 {vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment} },
		vk::DescriptorSetLayoutCreateFlags());

	m_descriptorPool = createDescriptorPool(m_framework->m_device,
		{ vk::DescriptorPoolSize(cameraUniformBuffer->getDescriptorType(), m_framework->m_maxFrames),
		vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, m_framework->m_maxFrames) });

	std::vector<vk::DescriptorSetLayout> layouts(m_framework->m_maxFrames, m_descriptorSetLayout.get());

	m_descriptorSets = m_framework->m_device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(m_descriptorPool.get(), layouts.size(), layouts.data()));

	std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers{ { cameraUniformBuffer } };
	std::vector<std::shared_ptr<Texture>> textures{ { m_textureCube } };

	updateDescriptorSets(m_framework->m_device, m_descriptorPool, m_descriptorSetLayout, m_framework->m_maxFrames, m_descriptorSets, uniformBuffers, textures);

	auto vertexShader = m_framework->getVulkanVertexShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("skybox.vert.spv").string()));
	auto fragmentShader = m_framework->getVulkanFragmentShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("skybox.frag.spv").string()));

	std::vector<ShaderInfo> shaderInfos = { { ShaderInfo{ vertexShader->shaderModule.get(),vk::SpecializationInfo(), vertexShader->getVulkanShaderType() },
											ShaderInfo{ fragmentShader->shaderModule.get(),vk::SpecializationInfo(), fragmentShader->getVulkanShaderType() } } };

	m_graphicPipeline = std::make_unique<VulkanGraphicPipelineInfo<Common::VertexP>>(m_framework, m_descriptorSetLayout, shaderInfos, vk::PrimitiveTopology::eTriangleList, true, vk::CullModeFlagBits::eNone, false, false, vk::CompareOp::eLessOrEqual);
}

void GraphicEngine::Vulkan::VulkanSkyboxGraphicPipeline::draw(vk::UniqueCommandBuffer& commandBuffer, int index)
{
	commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicPipeline->graphicPipeline.get());
	commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_graphicPipeline->pipelineLayout.get(), 0, 1, &m_descriptorSets[index].get(), 0, nullptr);

	m_cubeVertexBuffer->bind(commandBuffer);
	m_cubeVertexBuffer->draw(commandBuffer);
}

std::unique_ptr<GraphicEngine::Vulkan::VertexBuffer<GraphicEngine::Common::VertexP>> GraphicEngine::Vulkan::VulkanSkyboxGraphicPipeline::produceVertexBuffer(std::vector<Common::VertexP> skyBoxVertices)
{
	auto ptr = std::make_unique<VertexBuffer<Common::VertexP>>(m_framework->m_physicalDevice, m_framework->m_device, m_framework->m_commandPool, m_framework->m_graphicQueue, skyBoxVertices);
	return ptr;
}

std::shared_ptr<GraphicEngine::Vulkan::TextureCube> GraphicEngine::Vulkan::VulkanSkyboxGraphicPipeline::produceTextureCube(std::array<std::string, 6> faces)
{
	return std::make_shared<TextureCube>(m_framework->m_physicalDevice, m_framework->m_device, m_framework->m_commandPool, m_framework->m_graphicQueue, faces);
}
