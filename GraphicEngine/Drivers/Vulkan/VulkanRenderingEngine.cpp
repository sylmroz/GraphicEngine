#include "VulkanRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"
#include "../../Core/IO/FileSystem.hpp"

#include "VulkanTextureFactory.hpp"
#include "VulkanVertexBufferFactory.hpp"

#undef max

GraphicEngine::Vulkan::VulkanRenderingEngine::VulkanRenderingEngine(std::shared_ptr<VulkanWindowContext> vulkanWindowContext,
	std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Core::Configuration> cfg,
	std::unique_ptr<Core::Logger<VulkanRenderingEngine>> logger) :
	m_vulkanWindowContext(vulkanWindowContext),
	RenderingEngine(cameraControllerManager, eventManager, cfg)
{
}

bool GraphicEngine::Vulkan::VulkanRenderingEngine::drawFrame()
{
	try
	{
		m_framework->acquireFrame();

		m_uniformBuffer->updateAndSet(m_cameraControllerManager->getActiveCamera()->getViewProjectionMatrix());
		/*light.eyePosition = m_cameraControllerManager->getActiveCamera()->getPosition();
		m_lightUniformBuffer->updateAndSet(light);
		m_modelMatrix->updateAndSet({ Engines::Graphic::Shaders::ModelMartices(m_models.front()->getModelMatrix(), glm::transpose(glm::inverse(glm::mat3(m_cameraControllerManager->getActiveCamera()->getViewMatrix() * m_models.front()->getModelMatrix())))),
			 Engines::Graphic::Shaders::ModelMartices( glm::identity<glm::mat4>(), glm::transpose(glm::inverse(glm::mat3(m_cameraControllerManager->getActiveCamera()->getViewMatrix())))) });*/
		m_wireframeGraphicPipeline->updateDynamicUniforms();
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
		m_framework = std::make_shared<VulkanFramework>();
		m_framework->
			initialize(m_vulkanWindowContext, "Graphic Engine", "Vulkan Base", width, height, vk::SampleCountFlagBits::e2, { "VK_LAYER_KHRONOS_validation" }, std::make_unique<Core::Logger<VulkanFramework>>())
			.initializeCommandBuffer()
			.initializeFramebuffer()
			.initalizeRenderingBarriers();

		m_uniformBuffer = m_framework->getUniformBuffer<UniformBuffer, glm::mat4>();

		m_wireframeGraphicPipeline = std::make_shared<VulkanWireframeGraphicPipeline>(m_framework, m_uniformBuffer, m_cameraControllerManager);

		/*m_lightUniformBuffer = m_framework->getUniformBuffer<UniformBuffer, Engines::Graphic::Shaders::Light>();
		m_modelMatrix = m_framework->getUniformBuffer<UniformBufferDynamic, Engines::Graphic::Shaders::ModelMartices>(2);*/

		for (auto& model : m_models)
		{
			for (auto mesh : model->getMeshes())
			{
				auto vb = mesh->compile<VertexBufferFactory, VertexBuffer>(m_framework->m_physicalDevice, m_framework->m_device, m_framework->m_commandPool, m_framework->m_graphicQueue);
				m_wireframeGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
			}
		}


		for (auto& model : m_model2)
		{
			for (auto mesh : model->getMeshes())
			{
				auto vb = mesh->compile<VertexBufferFactory, VertexBuffer>(m_framework->m_physicalDevice, m_framework->m_device, m_framework->m_commandPool, m_framework->m_graphicQueue);
				m_wireframeGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
			}
		}

		/*m_descriptorSetLayout = createDescriptorSetLayout(m_framework->m_device,
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

		m_vertexShader = m_framework->getVulkanVertexShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("diffuse.vert.spv").string()));
		m_fragmentShader = m_framework->getVulkanFragmentShader(Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("diffuse.frag.spv").string()));

		m_graphicPipeline = createGraphicPipeline(m_framework->m_device, m_pipelineCache, 
			{ ShaderInfo{ m_vertexShader->shaderModule.get(),vk::SpecializationInfo(), m_vertexShader->getVulkanShaderType() },
			ShaderInfo{ m_fragmentShader->shaderModule.get(),vk::SpecializationInfo(), m_fragmentShader->getVulkanShaderType() } }, 
			createVertexInputAttributeDescriptions(decltype(m_models)::value_type::element_type::vertex_type::getSizeAndOffsets()),
			vk::VertexInputBindingDescription(0, decltype(m_models)::value_type::element_type::vertex_type::getStride()), true, vk::FrontFace::eClockwise, m_pipelineLayout, m_framework->m_renderPass, m_framework->m_msaaSamples);

		m_graphicPipeline2 = createGraphicPipeline(m_framework->m_device, m_pipelineCache,
			{ ShaderInfo{ m_vertexShader->shaderModule.get(),vk::SpecializationInfo(), m_vertexShader->getVulkanShaderType() },
			ShaderInfo{ m_fragmentShader->shaderModule.get(),vk::SpecializationInfo(), m_fragmentShader->getVulkanShaderType() } },
			createVertexInputAttributeDescriptions(decltype(m_model2)::value_type::element_type::vertex_type::getSizeAndOffsets()),
			vk::VertexInputBindingDescription(0, decltype(m_model2)::value_type::element_type::vertex_type::getStride()), true, vk::FrontFace::eClockwise, m_pipelineLayout, m_framework->m_renderPass, m_framework->m_msaaSamples);*/

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

		m_wireframeGraphicPipeline->draw(commandBuffer, i);

		/*commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicPipeline.get());

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

		commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicPipeline2.get());
		offset = getDynamicAligmentSize<Engines::Graphic::Shaders::ModelMartices>(m_framework->m_physicalDevice);
		commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout.get(), 0, 1, &m_descriptorSets[i].get(), 1, &offset);

		for (auto& vbs : m_vertexBuffers2)
		{
			for (auto& vb : vbs)
			{
				vb->bind(commandBuffer);
				vb->draw(commandBuffer);
			}
		}*/

		commandBuffer->endRenderPass();

		commandBuffer->end();
		++i;
	}
}
