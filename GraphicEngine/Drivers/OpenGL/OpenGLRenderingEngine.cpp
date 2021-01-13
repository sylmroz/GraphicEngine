#include "OpenGLRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"
#include "../../Core/IO/FileSystem.hpp"

#include "OpenGLTextureFactory.hpp"
#include "OpenGLVertexBufferFactory.hpp"

GraphicEngine::OpenGL::OpenGLRenderingEngine::OpenGLRenderingEngine(
	std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
	std::shared_ptr<Services::ModelManager> modelManager,
	std::shared_ptr<Services::LightManager> lightManager,
	std::shared_ptr<Services::ViewportManager> viewportManager,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Common::UI> ui,
	std::shared_ptr<Core::Configuration> cfg,
	std::unique_ptr<Core::Logger<OpenGLRenderingEngine>> logger) :
	m_logger(std::move(logger)),
	RenderingEngine(cameraControllerManager, modelManager, lightManager, viewportManager, eventManager, ui, cfg)
{
	m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Create OpenGL rendering engine instance.");
}

bool GraphicEngine::OpenGL::OpenGLRenderingEngine::drawFrame()
{
	glClearColor(m_viewportManager->backgroudColor.r, m_viewportManager->backgroudColor.g, m_viewportManager->backgroudColor.b, m_viewportManager->backgroudColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_ui->nextFrame();

	auto viewMatrix = m_cameraControllerManager->getActiveCamera()->getViewMatrix();
	auto projectionMatrix = m_cameraControllerManager->getActiveCamera()->getProjectionMatrix();

	Engines::Graphic::Shaders::CameraMatrices cameraMatrices{ viewMatrix, projectionMatrix };
	
	m_cameraUniformBuffer->update(&cameraMatrices);
	
	if (m_viewportManager->displayNormal)
		m_normalDebugGraphicPipeline->draw();
	if (m_viewportManager->displayWireframe)
		m_wireframeGraphicPipeline->draw();
	if (m_viewportManager->displaySolid)
		m_solidColorGraphicPipeline->draw();
	if(m_viewportManager->displaySkybox)
		m_skyboxGraphicPipeline->draw();

	m_ui->drawUi();
	m_uiRenderingBackend->renderData();

	return false;
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::init(size_t width, size_t height)
{
	m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Initialize OpenGL rendering engine instance.");
	
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("OpenGL initialization failed!");
	}
	glViewport(0, 0, width, height);

	try
	{
		m_wireframeGraphicPipeline = std::make_unique<OpenGLWireframeGraphicPipeline>(m_cameraControllerManager);
		m_solidColorGraphicPipeline = std::make_unique<OpenGLSolidColorGraphicPipeline>(m_cameraControllerManager);
		m_normalDebugGraphicPipeline = std::make_unique<OpenGLNormalDebugGraphicPipeline>(m_cameraControllerManager);
		m_skyboxGraphicPipeline = std::make_unique<OpenGLSkyboxGraphicPipeline>(m_cfg->getProperty<std::string>("scene:skybox:base path"));

		m_cameraUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>>(0);

		m_directionalLight = std::make_shared<ShaderStorageBufferObject<Engines::Graphic::Shaders::DirectionalLight>>(7);
		m_pointLights = std::make_shared<ShaderStorageBufferObject<Engines::Graphic::Shaders::PointLight>>(8);
		m_spotLight = std::make_shared<ShaderStorageBufferObject<Engines::Graphic::Shaders::SpotLight>>(9);

		m_directionalLight->update(m_lightManager->getDirectionalLights());

		m_pointLights->update(m_lightManager->getPointLights());

		m_spotLight->update(m_lightManager->getSpotLights());

		m_modelManager->getModelEntityContainer()->forEachEntity([&](auto model)
		{
			for (auto mesh : model->getMeshes())
			{
				auto vb = mesh->compile<VertexBufferFactory, VertexBuffer>();
				m_wireframeGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_solidColorGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_normalDebugGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
			}
		});

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glFrontFace(GL_CCW);

		m_uiRenderingBackend = std::make_shared<GUI::ImGuiImpl::OpenGlRenderEngineBackend>();
		m_ui->addBackend(m_uiRenderingBackend);
	}

	catch (std::runtime_error& err)
	{
		m_logger->error(__FILE__, __LINE__, __FUNCTION__, err.what());
		throw (err);
	}
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::resizeFrameBuffer(size_t width, size_t height)
{
	glViewport(0, 0, width, height);
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::cleanup()
{
}
