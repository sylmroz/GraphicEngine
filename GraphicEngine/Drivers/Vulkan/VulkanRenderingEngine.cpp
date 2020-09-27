#include "VulkanRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"
#include "../../Core/IO/FileSystem.hpp"

#include "VulkanTextureFactory.hpp"
#include "VulkanVertexBufferFactory.hpp"

#undef max

GraphicEngine::Vulkan::VulkanRenderingEngine::VulkanRenderingEngine(std::shared_ptr<VulkanWindowContext> vulkanWindowContext,
	std::shared_ptr<Common::Camera> camera,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Core::Configuration> cfg,
	std::unique_ptr<Core::Logger<VulkanRenderingEngine>> logger) :
	m_vulkanWindowContext(vulkanWindowContext),
	RenderingEngine(camera, eventManager, cfg)
{
}

bool GraphicEngine::Vulkan::VulkanRenderingEngine::drawFrame()
{
	try
	{
		m_framework->acquireFrame();

		m_uniformBuffer->updateAndSet(m_camera->getViewProjectionMatrix());
		light.eyePosition = m_camera->getPosition();
		m_lightUniformBuffer->updateAndSet(light);
		m_modelMatrix->updateAndSet({ Scene::ModelMartices(m_models.front()->getModelMatrix(), glm::transpose(glm::inverse(glm::mat3(m_camera->getViewMatrix() * m_models.front()->getModelMatrix())))),
			 Scene::ModelMartices( glm::identity<glm::mat4>(), glm::transpose(glm::inverse(glm::mat3(m_camera->getViewMatrix())))) });

		m_framework->submitFrame();
	}

	catch (vk::OutOfDateKHRError err)
	{
		return false;
	}

	return true;
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::init(size_t width, size_t height)
{
	try
	{
		m_framework = std::make_unique<VulkanFramework>();
		m_framework->
			initialize(m_vulkanWindowContext, "Graphic Engine", "Vulkan Base", width, height, vk::SampleCountFlagBits::e2, { "VK_LAYER_KHRONOS_validation" }, std::make_unique<Core::Logger<VulkanFramework>>())
			.initializeCommandBuffer()
			.initializeFramebuffer()
			.initalizeRenderingBarriers();

		m_uniformBuffer = m_framework->getUniformBuffer<UniformBuffer, glm::mat4>();
		m_lightUniformBuffer = m_framework->getUniformBuffer<UniformBuffer, Light>();
		m_modelMatrix = m_framework->getUniformBuffer<UniformBufferDynamic, Scene::ModelMartices>(2);

		for (auto& model : m_models)
		{
			m_vertexBuffers.push_back(model->compile<VertexBufferFactory, VertexBuffer>(m_framework->m_physicalDevice, m_framework->m_device, m_framework->m_commandPool, m_framework->m_graphicQueue));
		}
		
		m_vertexShader = std::make_unique<VulkanVertexShader>(m_framework->m_device, Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("diffuse.vert.spv").string()));
		m_fragmentShader = std::make_unique<VulkanFragmentShader>(m_framework->m_device, Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("diffuse.frag.spv").string()));

		m_descriptorSetLayout = createDescriptorSetLayout(m_framework->m_device,
			{ {m_uniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eVertex},
			 {m_lightUniformBuffer->getDescriptorType(), 1, vk::ShaderStageFlagBits::eFragment},
			 {m_modelMatrix->getDescriptorType(), 1, vk::ShaderStageFlagBits::eVertex} },
			vk::DescriptorSetLayoutCreateFlags());

		m_descriptorPool = createDescriptorPool(m_framework->m_device,
			{ vk::DescriptorPoolSize(m_uniformBuffer->getDescriptorType(), m_framework->m_maxFrames),
			vk::DescriptorPoolSize(m_lightUniformBuffer->getDescriptorType(), m_framework->m_maxFrames),
			vk::DescriptorPoolSize(m_modelMatrix->getDescriptorType(), m_framework->m_maxFrames) });

		std::vector<vk::DescriptorSetLayout> layouts(m_framework->m_maxFrames, m_descriptorSetLayout.get());

		m_descriptorSets = m_framework->m_device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(m_descriptorPool.get(), layouts.size(), layouts.data()));

		std::vector<std::shared_ptr<IUniformBuffer>> uniformBuffers{ {m_uniformBuffer, m_lightUniformBuffer, m_modelMatrix} };

		updateDescriptorSets(m_framework->m_device, m_descriptorPool, m_descriptorSetLayout, m_framework->m_maxFrames, m_descriptorSets, uniformBuffers, {});

		m_pipelineLayout = m_framework->m_device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &m_descriptorSetLayout.get()));

		m_pipelineCache = m_framework->m_device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());

		m_graphicPipeline = createGraphicPipeline(m_framework->m_device, m_pipelineCache, ShaderInfo{ m_vertexShader->shaderModule.get(),vk::SpecializationInfo() },
			ShaderInfo{ m_fragmentShader->shaderModule.get(),vk::SpecializationInfo() }, createVertexInputAttributeDescriptions(Common::VertexPN::getSizeAndOffsets()),
			vk::VertexInputBindingDescription(0, Common::VertexPN::getStride()), true, vk::FrontFace::eClockwise, m_pipelineLayout, m_framework->m_renderPass, m_framework->m_msaaSamples, vk::CullModeFlagBits::eNone);

		buildCommandBuffers();
	}

	catch (vk::SystemError& err)
	{
		throw std::runtime_error(err.what());
	}

	catch (std::runtime_error& err)
	{
		throw std::runtime_error(err.what());
	}

	catch (...)
	{
		throw std::runtime_error("Vulkan Initialize: unknown error\n");
	}
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::resizeFrameBuffer(size_t width, size_t height)
{
	if (width == 0 || height == 0)
		return;

	m_framework->initializeFramebuffer(width, height);

	buildCommandBuffers();
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::cleanup()
{
	m_framework->m_graphicQueue.waitIdle();
	m_framework->m_device->waitIdle();
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::buildCommandBuffers()
{
	std::array<vk::ClearValue, 3> clearValues;

	clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f,0.2f,0.2f,1.0f }));
	clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0.0f);
	clearValues[2].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f,0.2f,0.2f,1.0f }));
	int i{ 0 };
	for (auto& commandBuffer : m_framework->m_commandBuffers)
	{
		commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlags()));

		commandBuffer->setViewport(0, vk::Viewport(0.0f, static_cast<float>(m_framework->m_swapChainData.extent.height),
			static_cast<float>(m_framework->m_swapChainData.extent.width), -static_cast<float>(m_framework->m_swapChainData.extent.height), 0.0f, 1.0f));
		commandBuffer->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_framework->m_swapChainData.extent));

		vk::RenderPassBeginInfo renderPassBeginInfo(m_framework->m_renderPass.get(), m_framework->m_frameBuffers[i].get(), vk::Rect2D(vk::Offset2D(0, 0), m_framework->m_swapChainData.extent), static_cast<uint32_t>(clearValues.size()), clearValues.data());
		commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

		commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicPipeline.get());

		uint32_t offset{ 0 };
		commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout.get(), 0, 1, &m_descriptorSets[i].get(), 1, &offset);

		for (auto& vbs : m_vertexBuffers)
		{
			for (auto& vb : vbs)
			{
				vb->bind(commandBuffer);
				vb->draw(commandBuffer);
			}
		}

		offset = getDynamicAligmentSize<Scene::ModelMartices>(m_framework->m_physicalDevice);
		commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout.get(), 0, 1, &m_descriptorSets[i].get(), 1, &offset);

		for (auto& vbs : m_vertexBuffers)
		{
			for (auto& vb : vbs)
			{
				vb->bind(commandBuffer);
				vb->draw(commandBuffer);
			}
		}

		commandBuffer->endRenderPass();

		commandBuffer->end();
		++i;
	}
}
