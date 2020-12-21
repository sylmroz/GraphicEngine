#include "OpenGLRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"
#include "../../Core/IO/FileSystem.hpp"

#include "OpenGLTextureFactory.hpp"
#include "OpenGLVertexBufferFactory.hpp"

GraphicEngine::OpenGL::OpenGLRenderingEngine::OpenGLRenderingEngine(
	std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
	std::shared_ptr<Services::ModelManager> modelManager,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Common::UI> ui,
	std::shared_ptr<Core::Configuration> cfg,
	std::unique_ptr<Core::Logger<OpenGLRenderingEngine>> logger) :
	m_logger(std::move(logger)),
	RenderingEngine(cameraControllerManager, modelManager, eventManager, ui, cfg)
{
	m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Create OpenGL rendering engine instance.");
}

bool GraphicEngine::OpenGL::OpenGLRenderingEngine::drawFrame()
{
	glClearColor(backgroudColor.r, backgroudColor.g, backgroudColor.b, backgroudColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_ui->nextFrame();
	m_wireframeGraphicPipeline->draw();
	m_solidColorGraphicPipeline->draw();

	m_ui->drawUi();
	m_uiRenderingBackend->renderData();

	return false;
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::init(size_t width, size_t height)
{
	m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Initialize OpenGL rendering engine instance.");

	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("OpenGL initialization failed!");
	}
	glViewport(0, 0, width, height);

	try
	{
		m_wireframeGraphicPipeline = std::make_unique<OpenGLWireframeGraphicPipeline>();
		m_solidColorGraphicPipeline = std::make_unique<OpenGLSolidColorGraphicPipeline>(m_cameraControllerManager);
		m_modelManager->getModelEntityContainer()->forEachEntity([&](auto model)
		{
			for (auto mesh : model->getMeshes())
			{
				auto vb = mesh->compile<VertexBufferFactory, VertexBuffer>();
				m_wireframeGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_solidColorGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
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
