#include "VulkanNormalDebugGraphicPileline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

GraphicEngine::Vulkan::VulkanNormalDebugGraphicPipeline::VulkanNormalDebugGraphicPipeline(std::shared_ptr<VulkanFramework> framework, std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> cameraUniformBuffer,
	std::shared_ptr<Services::CameraControllerManager> cameraControllerManager) :
	m_framework{ framework }
{
	m_cameraControllerManager = cameraControllerManager;
	m_vulkanGraphicPipelines = std::make_shared<Common::EntityByVertexTypeManager<VulkanGraphicPipelineInfo>>();
	m_cameraUniformBuffer = cameraUniformBuffer;
	m_modelDescriptorUniformBuffer = m_framework->getUniformBuffer<UniformBufferDynamic, Engines::Graphic::Shaders::ModelMartices>(1);

	alignedSize = getDynamicAligmentSize<Engines::Graphic::Shaders::ModelMartices>(m_framework->m_physicalDevice);

	m_descriptorSetLayout = createDescriptorSetLayout(m_framework->m_device,
		{ {m_cameraUniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eVertex},
		 {m_modelDescriptorUniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eVertex} },
		vk::DescriptorSetLayoutCreateFlags());

	m_descriptorPool = createDescriptorPool(m_framework->m_device,
		{ vk::DescriptorPoolSize(m_cameraUniformBuffer->getDescriptorType(), m_framework->m_maxFrames),
		vk::DescriptorPoolSize(m_modelDescriptorUniformBuffer->getDescriptorType(), m_framework->m_maxFrames) });

	std::vector<vk::DescriptorSetLayout> layouts(m_framework->m_maxFrames, m_descriptorSetLayout.get());

	m_descriptorSets = m_framework->m_device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(m_descriptorPool.get(), layouts.size(), layouts.data()));

	std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers{ {m_cameraUniformBuffer, m_modelDescriptorUniformBuffer} };

	updateDescriptorSets(m_framework->m_device, m_descriptorPool, m_descriptorSetLayout, m_framework->m_maxFrames, m_descriptorSets, uniformBuffers, {});

	auto vertexShader = m_framework->getVulkanVertexShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("normals.vert.spv").string()));
	auto geometryShader = m_framework->getVulkanGeometryShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("normals.geom.spv").string()));
	auto fragmentShader = m_framework->getVulkanFragmentShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("normals.frag.spv").string()));

	std::vector<ShaderInfo> shaderInfos = { { ShaderInfo{ vertexShader->shaderModule.get(),vk::SpecializationInfo(), vertexShader->getVulkanShaderType() },
											ShaderInfo{ geometryShader->shaderModule.get(),vk::SpecializationInfo(), geometryShader->getVulkanShaderType() },
											ShaderInfo{ fragmentShader->shaderModule.get(),vk::SpecializationInfo(), fragmentShader->getVulkanShaderType() } } };

	Core::Utils::for_each(Common::VertexTypesRegister::types, [&](auto vertexType)
	{
		if constexpr (Core::Utils::has_normal_member<decltype(vertexType)>::value)
		{
			auto graphicPipeline = std::make_shared<VulkanGraphicPipelineInfo<decltype(vertexType)>>(m_framework, m_descriptorSetLayout, shaderInfos, vk::PrimitiveTopology::eTriangleList);
			m_vulkanGraphicPipelines->addEntity(graphicPipeline);
		}
	});
}

void GraphicEngine::Vulkan::VulkanNormalDebugGraphicPipeline::draw(vk::UniqueCommandBuffer& commandBuffer, int index)
{
	uint32_t offset{ 0 };

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		auto graphicPipeline = m_vulkanGraphicPipelines->getFirstEntity<decltype(vertexBufferCollection)::element_type::vertex_type>();

		commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicPipeline->graphicPipeline.get());
		commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, graphicPipeline->pipelineLayout.get(), 0, 1, &m_descriptorSets[index].get(), 1, &offset);

		vertexBufferCollection->vertexBuffer->drawElements(commandBuffer);

		offset += alignedSize;
	});
}

void GraphicEngine::Vulkan::VulkanNormalDebugGraphicPipeline::updateDynamicUniforms()
{
	uint32_t i{ 0 };
	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
		vertexBufferCollection->modelDescriptor.normalMatrix = glm::transpose(glm::inverse(m_cameraControllerManager->getActiveCamera()->getViewMatrix() * vertexBufferCollection->modelDescriptor.modelMatrix));
		m_modelDescriptors[i] = vertexBufferCollection->modelDescriptor;
		++i;
	});
	m_modelDescriptorUniformBuffer->updateAndSet(m_modelDescriptors);
}

void GraphicEngine::Vulkan::VulkanNormalDebugGraphicPipeline::addUniformBuffer()
{
	if (m_modelDescriptors.size() > 0)
	{
		m_modelDescriptorUniformBuffer->addInstance();

		std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers{ {m_cameraUniformBuffer, m_modelDescriptorUniformBuffer} };
		updateDescriptorSets(m_framework->m_device, m_descriptorPool, m_descriptorSetLayout, m_framework->m_maxFrames, m_descriptorSets, uniformBuffers, {});
	}

	m_modelDescriptors.resize(m_modelDescriptors.size() + 1);
}

void GraphicEngine::Vulkan::VulkanNormalDebugGraphicPipeline::deleteUniformBuffer()
{
	if (m_modelDescriptors.size() > 0)
	{
		m_modelDescriptors.resize(m_modelDescriptors.size() - 1);
		if (m_modelDescriptors.size() > 0)
		{
			m_modelDescriptorUniformBuffer->deleteInstance();

			std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers{ {m_cameraUniformBuffer, m_modelDescriptorUniformBuffer} };
			updateDescriptorSets(m_framework->m_device, m_descriptorPool, m_descriptorSetLayout, m_framework->m_maxFrames, m_descriptorSets, uniformBuffers, {});
		}
	}
}
