#include "VulkanRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"
#include "../../Core/IO/FileSystem.hpp"

#include "VulkanTextureFactory.hpp"
#include "VulkanVertexBufferFactory.hpp"

#undef max

GraphicEngine::Vulkan::VulkanRenderingEngine::VulkanRenderingEngine(std::shared_ptr<VulkanWindowContext> vulkanWindowContext,
	std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
	std::shared_ptr<Services::ModelManager> modelManager,
	std::shared_ptr<Services::LightManager> lightManager,
	std::shared_ptr<Services::ViewportManager> viewportManager,
	std::shared_ptr<Services::RenderingOptionsManager> renderingOptionsManager,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Common::UI> ui,
	std::shared_ptr<Core::Configuration> cfg,
	std::unique_ptr<Core::Logger<VulkanRenderingEngine>> logger) :
	m_vulkanWindowContext(vulkanWindowContext),
	RenderingEngine(cameraControllerManager, modelManager, lightManager, viewportManager, renderingOptionsManager, eventManager, ui, cfg)
{
}

bool GraphicEngine::Vulkan::VulkanRenderingEngine::drawFrame()
{
	try
	{
		m_framework->acquireFrame();
		buildCommandBuffers();

		auto view = m_cameraControllerManager->getActiveCamera()->getViewMatrix();
		auto projection = m_cameraControllerManager->getActiveCamera()->getProjectionMatrix();

		glm::vec4 eyePosition = glm::vec4(m_cameraControllerManager->getActiveCamera()->getPosition(), 1.0);

		Engines::Graphic::Shaders::Light light{ eyePosition, glm::vec4{ 1.0f } };
		Engines::Graphic::Shaders::Eye eye{ eyePosition };

		Engines::Graphic::Shaders::CameraMatrices cameraMatrix(view, projection);
		m_cameraUniformBuffer->updateAndSet(cameraMatrix);
		m_eyePositionUniformBuffer->updateAndSet(eye);
		m_directionalLight->update(m_lightManager->getDirectionalLights());
		//m_pointLights->update(m_lightManager->getPointLights());
		//m_spotLight->update(m_lightManager->getSpotLights());

		if (m_viewportManager->displayNormal)
			m_normalDebugGraphicPipeline->updateDynamicUniforms();
		if (m_viewportManager->displayWireframe)
			m_wireframeGraphicPipeline->updateDynamicUniforms();
		if (m_viewportManager->displaySolid)
			m_solidColorraphicPipeline->updateDynamicUniforms();
		

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

		m_cameraUniformBuffer = m_framework->getUniformBuffer<UniformBuffer,Engines::Graphic::Shaders::CameraMatrices>();
		m_directionalLight = std::make_shared<ShaderStorageBufferObject<Engines::Graphic::Shaders::DirectionalLight>>(m_framework.get());
		m_pointLights = std::make_shared<ShaderStorageBufferObject<Engines::Graphic::Shaders::PointLight>>(m_framework.get());
		m_spotLight = std::make_shared<ShaderStorageBufferObject<Engines::Graphic::Shaders::SpotLight>>(m_framework.get());
		
		m_eyePositionUniformBuffer = m_framework->getUniformBuffer<UniformBuffer, Engines::Graphic::Shaders::Eye>();

		m_wireframeGraphicPipeline = std::make_shared<VulkanWireframeGraphicPipeline>(m_framework, m_cameraUniformBuffer);
		m_solidColorraphicPipeline = std::make_shared<VulkanSolidColorGraphicPipeline>(m_framework, m_cameraUniformBuffer, m_eyePositionUniformBuffer, m_directionalLight, m_pointLights, m_spotLight, m_cameraControllerManager);
		m_normalDebugGraphicPipeline = std::make_shared<VulkanNormalDebugGraphicPipeline>(m_framework, m_cameraUniformBuffer, m_cameraControllerManager);
		m_skyboxGraphicPipeline = std::make_unique<VulkanSkyboxGraphicPipeline>(m_framework, m_cameraUniformBuffer, m_cfg->getProperty<std::string>("scene:skybox:texture path"));

		m_modelManager->getModelEntityContainer()->forEachEntity([&](auto model)
		{
			for (auto mesh : model->getMeshes())
			{
				auto vb = mesh->compile<VertexBufferFactory, VertexBuffer>(m_framework->m_physicalDevice, m_framework->m_device, m_framework->m_commandPool, m_framework->m_graphicQueue);
				m_wireframeGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_solidColorraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_normalDebugGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
			}
		});
		m_uiRenderingBackend = std::make_shared<GUI::ImGuiImpl::VulkanRenderEngineBackend>(m_framework);
		m_ui->addBackend(m_uiRenderingBackend);
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

	clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ m_viewportManager->backgroudColor.r, m_viewportManager->backgroudColor.g, m_viewportManager->backgroudColor.b, m_viewportManager->backgroudColor.a }));
	clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0.0f);
	clearValues[2].color = vk::ClearColorValue(std::array<float, 4>({ m_viewportManager->backgroudColor.r, m_viewportManager->backgroudColor.g, m_viewportManager->backgroudColor.b, m_viewportManager->backgroudColor.a }));
	int i{ 0 };
	m_framework->m_device->waitIdle();

	m_ui->nextFrame();
	m_ui->drawUi();

	for (auto& commandBuffer : m_framework->m_commandBuffers)
	{
		commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlags()));

		commandBuffer->setViewport(0, vk::Viewport(0.0f, static_cast<float>(m_framework->m_swapChainData.extent.height),
			static_cast<float>(m_framework->m_swapChainData.extent.width), -static_cast<float>(m_framework->m_swapChainData.extent.height), 0.0f, 1.0f));
		commandBuffer->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_framework->m_swapChainData.extent));

		vk::RenderPassBeginInfo renderPassBeginInfo(m_framework->m_renderPass.get(), m_framework->m_frameBuffers[i].get(), vk::Rect2D(vk::Offset2D(0, 0), m_framework->m_swapChainData.extent), static_cast<uint32_t>(clearValues.size()), clearValues.data());
		commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

		if (m_viewportManager->displayNormal)
			m_normalDebugGraphicPipeline->draw(commandBuffer, i);
		if (m_viewportManager->displayWireframe)
			m_wireframeGraphicPipeline->draw(commandBuffer, i);
		if (m_viewportManager->displaySolid)
			m_solidColorraphicPipeline->draw(commandBuffer, i);
		if (m_viewportManager->displaySkybox)
			m_skyboxGraphicPipeline->draw(commandBuffer, i);

		m_uiRenderingBackend->renderData(commandBuffer);

		commandBuffer->endRenderPass();

		commandBuffer->end();
		++i;
	}
}
