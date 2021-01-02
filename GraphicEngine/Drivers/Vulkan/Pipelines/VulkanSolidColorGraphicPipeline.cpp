#include "VulkanSolidColorGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"
#include "../../../Core/Utils/MemberTraits.hpp"

GraphicEngine::Vulkan::VulkanSolidColorGraphicPipeline::VulkanSolidColorGraphicPipeline(std::shared_ptr<VulkanFramework> framework, std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> cameraUniformBuffer,
	std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::Light>> lightUniformBuffer, std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::Eye>> eyePositionUniformBuffer,
	std::shared_ptr<Services::CameraControllerManager> cameraControllerManager):
	Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>{ cameraControllerManager },
	m_framework{ framework }
{
	m_cameraUniformBuffer = cameraUniformBuffer;
	m_ligthUniformBuffer = lightUniformBuffer;
	m_eyePositionUniformBuffer = eyePositionUniformBuffer;

	m_vulkanGraphicPipelines = std::make_shared<Common::EntityByVertexTypeManager<VulkanGraphicPipelineInfo>>();
	
	m_solidColorUniformBuffer = m_framework->getUniformBuffer<UniformBufferDynamic, Engines::Graphic::Shaders::SolidColorModelDescriptor>(1);

	alignedSize = getDynamicAligmentSize<Engines::Graphic::Shaders::SolidColorModelDescriptor>(m_framework->m_physicalDevice);

	m_descriptorSetLayout = createDescriptorSetLayout(m_framework->m_device,
		{ 
			{ m_cameraUniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eVertex },
			{ m_ligthUniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eFragment },
			{ m_eyePositionUniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eFragment },
			{ m_solidColorUniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eVertex }
		},
		vk::DescriptorSetLayoutCreateFlags());

	m_descriptorPool = createDescriptorPool(m_framework->m_device,
		{ 
			vk::DescriptorPoolSize(m_cameraUniformBuffer->getDescriptorType(), m_framework->m_maxFrames),
			vk::DescriptorPoolSize(m_ligthUniformBuffer->getDescriptorType(), m_framework->m_maxFrames),
			vk::DescriptorPoolSize(m_eyePositionUniformBuffer->getDescriptorType(), m_framework->m_maxFrames),
			vk::DescriptorPoolSize(m_solidColorUniformBuffer->getDescriptorType(), m_framework->m_maxFrames) 
		});

	std::vector<vk::DescriptorSetLayout> layouts(m_framework->m_maxFrames, m_descriptorSetLayout.get());

	m_descriptorSets = m_framework->m_device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(m_descriptorPool.get(), layouts.size(), layouts.data()));

	std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers{ {m_cameraUniformBuffer, m_ligthUniformBuffer, m_eyePositionUniformBuffer, m_solidColorUniformBuffer} };

	updateDescriptorSets(m_framework->m_device, m_descriptorPool, m_descriptorSetLayout, m_framework->m_maxFrames, m_descriptorSets, uniformBuffers, {});

	auto vertexShader = m_framework->getVulkanVertexShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("solid.vert.spv").string()));
	auto fragmentShader = m_framework->getVulkanFragmentShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("solid.frag.spv").string()));

	std::vector<ShaderInfo> shaderInfos = { { ShaderInfo{ vertexShader->shaderModule.get(),vk::SpecializationInfo(), vertexShader->getVulkanShaderType() },
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

void GraphicEngine::Vulkan::VulkanSolidColorGraphicPipeline::draw(vk::UniqueCommandBuffer& commandBuffer, int index)
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

void GraphicEngine::Vulkan::VulkanSolidColorGraphicPipeline::updateDynamicUniforms()
{
	uint32_t i{ 0 };
	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
	{
		vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
		vertexBufferCollection->modelDescriptor.normalMatrix = glm::transpose(glm::inverse(m_cameraControllerManager->getActiveCamera()->getViewMatrix() * vertexBufferCollection->modelDescriptor.modelMatrix));
		vertexBufferCollection->modelDescriptor.color = vertexBufferCollection->mesh->getMaterial().solidColor;
		m_solidColorModelDescriptors[i] = vertexBufferCollection->modelDescriptor;
		++i;
	});
	m_solidColorUniformBuffer->updateAndSet(m_solidColorModelDescriptors);
}

void GraphicEngine::Vulkan::VulkanSolidColorGraphicPipeline::addUniformBuffer()
{
	if (m_solidColorModelDescriptors.size() > 0)
	{
		m_solidColorUniformBuffer->addInstance();

		std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers{ {m_cameraUniformBuffer, m_ligthUniformBuffer, m_eyePositionUniformBuffer, m_solidColorUniformBuffer} };
		updateDescriptorSets(m_framework->m_device, m_descriptorPool, m_descriptorSetLayout, m_framework->m_maxFrames, m_descriptorSets, uniformBuffers, {});
	}

	m_solidColorModelDescriptors.resize(m_solidColorModelDescriptors.size() + 1);
}

void GraphicEngine::Vulkan::VulkanSolidColorGraphicPipeline::deleteUniformBuffer()
{
	if (m_solidColorModelDescriptors.size() > 0)
	{
		m_solidColorModelDescriptors.resize(m_solidColorModelDescriptors.size() - 1);
		if (m_solidColorModelDescriptors.size() > 0)
		{
			m_solidColorUniformBuffer->deleteInstance();

			std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers{ {m_cameraUniformBuffer, m_ligthUniformBuffer, m_eyePositionUniformBuffer, m_solidColorUniformBuffer} };
			updateDescriptorSets(m_framework->m_device, m_descriptorPool, m_descriptorSetLayout, m_framework->m_maxFrames, m_descriptorSets, uniformBuffers, {});
		}
	}
}
