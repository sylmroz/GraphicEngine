#include "VulkanWireframeGraphicPipeline.h"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::Vulkan::VulkanWireframeGraphicPipeline::VulkanWireframeGraphicPipeline(std::shared_ptr<VulkanFramework> framework, std::shared_ptr<UniformBuffer<glm::mat4>> cameraUniformBuffer, std::shared_ptr<Services::CameraControllerManager> cameraControllerManager) :
	WireframeGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>{ cameraControllerManager },
	m_framework{ framework }
{
	m_vulkanGraphicPipelines = std::make_shared<Common::EntityByVertexTypeManager<VulkanGraphicPipelineInfo>>();
	m_cameraUniformBuffer = cameraUniformBuffer;
	m_wireframeModelDescriptorUniformBuffer = m_framework->getUniformBuffer<UniformBufferDynamic, Engines::Graphic::Shaders::WireframeModelDescriptor>(2); // TODO
	m_wireframeModelDescriptors.resize(2);// todo

	alignedSize = getDynamicAligmentSize<Engines::Graphic::Shaders::WireframeModelDescriptor>(m_framework->m_physicalDevice);

	m_descriptorSetLayout = createDescriptorSetLayout(m_framework->m_device,
		{ {m_cameraUniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eVertex},
		 {m_wireframeModelDescriptorUniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eVertex} },
		vk::DescriptorSetLayoutCreateFlags());

	m_descriptorPool = createDescriptorPool(m_framework->m_device,
		{ vk::DescriptorPoolSize(m_cameraUniformBuffer->getDescriptorType(), m_framework->m_maxFrames),
		vk::DescriptorPoolSize(m_wireframeModelDescriptorUniformBuffer->getDescriptorType(), m_framework->m_maxFrames) });

	std::vector<vk::DescriptorSetLayout> layouts(m_framework->m_maxFrames, m_descriptorSetLayout.get());

	m_descriptorSets = m_framework->m_device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(m_descriptorPool.get(), layouts.size(), layouts.data()));

	std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers{ {m_cameraUniformBuffer, m_wireframeModelDescriptorUniformBuffer} };

	updateDescriptorSets(m_framework->m_device, m_descriptorPool, m_descriptorSetLayout, m_framework->m_maxFrames, m_descriptorSets, uniformBuffers, {});

	auto vertexShader = m_framework->getVulkanVertexShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("wireframe.vert.spv").string()));
	auto fragmentShader = m_framework->getVulkanFragmentShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("wireframe.frag.spv").string()));

	std::vector<ShaderInfo> shaderInfos = { { ShaderInfo{ vertexShader->shaderModule.get(),vk::SpecializationInfo(), vertexShader->getVulkanShaderType() },
											ShaderInfo{ fragmentShader->shaderModule.get(),vk::SpecializationInfo(), fragmentShader->getVulkanShaderType() } } };

	Core::Utils::for_each(Common::VertexTypesRegister::types, [&](auto vertexType)
	{
		auto graphicPipeline = std::make_shared<VulkanGraphicPipelineInfo<decltype(vertexType)>>(m_framework, m_descriptorSetLayout, shaderInfos, vk::PrimitiveTopology::eLineList);
		m_vulkanGraphicPipelines->addEntity(graphicPipeline);
	});
}

void GraphicEngine::Vulkan::VulkanWireframeGraphicPipeline::draw(vk::UniqueCommandBuffer& commandBuffer, int index)
{
	uint32_t offset{ 0 };

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		auto graphicPipeline = m_vulkanGraphicPipelines->getFirstEntity<decltype(vertexBufferCollection)::element_type::vertex_type>();

		commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicPipeline->graphicPipeline.get());
		commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, graphicPipeline->pipelineLayout.get(), 0, 1, &m_descriptorSets[index].get(), 1, &offset);

		vertexBufferCollection->vertexBuffer->bind(commandBuffer);
		vertexBufferCollection->vertexBuffer->draw(commandBuffer);

		offset += alignedSize;
	});
}

void GraphicEngine::Vulkan::VulkanWireframeGraphicPipeline::updateDynamicUniforms()
{
	uint32_t i{ 0 };
	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
		m_wireframeModelDescriptors[i] = vertexBufferCollection->modelDescriptor;
		++i;
	});
	m_wireframeModelDescriptorUniformBuffer->updateAndSet(m_wireframeModelDescriptors);
}
