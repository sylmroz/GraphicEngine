#include "OpenGLRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"
#include "../../Core/IO/FileSystem.hpp"

#include "OpenGLTextureFactory.hpp"
#include "OpenGLVertexBufferFactory.hpp"

GraphicEngine::OpenGL::OpenGLRenderingEngine::OpenGLRenderingEngine(
	std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Core::Configuration> cfg,
	std::unique_ptr<Core::Logger<OpenGLRenderingEngine>> logger) :
	m_logger(std::move(logger)),
	RenderingEngine(cameraControllerManager, eventManager, cfg)
{
	m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Create OpenGL rendering engine instance.");
}

bool GraphicEngine::OpenGL::OpenGLRenderingEngine::drawFrame()
{
	glClearColor(backgroudColor.r, backgroudColor.g, backgroudColor.b, backgroudColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//auto v = m_cameraControllerManager->getActiveCamera()->getViewProjectionMatrix();
	//m_uniformBufferMatrix->update(&v);

	//light.eyePosition = m_cameraControllerManager->getActiveCamera()->getPosition();
	//m_lightUniformBuffer->update(&light);
	//Engines::Graphic::Shaders::ModelMartices m(m_models.front()->getMeshes().front()->getModelMatrix(), glm::transpose(glm::inverse(glm::mat3(m_cameraControllerManager->getActiveCamera()->getViewMatrix()/* * m_models.front()->getModelMatrix()*/))));
	//m_modelMatrix->update(&m);

	//m_program->use();

	//for (auto& vbs : m_vertexBuffers)
	//{
	//	for (auto& vb : vbs)
	//	{
	//		vb->bind();
	//		vb->draw(GL_TRIANGLES);
	//	}
	//}
	//m.modelMatrix = glm::identity<glm::mat4>();
	//m.normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_camera->getViewMatrix())));
	//m_modelMatrix->update(&m);

	//for (auto& vbs : m_vertexBuffers)
	//{
	//	for (auto& vb : vbs)
	//	{
	//		vb->bind();
	//		vb->draw();
	//	}
	//}

	m_wireframeGraphicPipeline->draw();

	return false;
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::init(size_t width, size_t height)
{
	m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Initialize OpenGL rendering engine instance.");
	using namespace GraphicEngine::Core::IO;
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("OpenGL initialization failed!");
	}
	glViewport(0, 0, width, height);

	try
	{
		/*OpenGLVertexShader vert(readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("diffuse.vert").string()));
		OpenGLFragmentShader frag(readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("diffuse.frag").string()));

		m_program = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });
		
		auto uniformIndex = glGetUniformBlockIndex(m_program->getShaderProgramId(), "MVP");
		glUniformBlockBinding(m_program->getShaderProgramId(), uniformIndex, 0);*/
		/*auto textureIndex = glGetUniformLocation(m_program->getShaderProgramId(), "texture1");
		glUniform1i(textureIndex, 0);*/

		/*m_uniformBufferMatrix = std::make_shared<UniformBuffer<glm::mat4>>(0);
		m_lightUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::Light>>(1);
		m_modelMatrix = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::ModelMartices>>(2);*/

		m_wireframeGraphicPipeline = std::make_unique<OpenGLWireframeGraphicPipeline>(m_cameraControllerManager);

		for (auto& model : m_models)
		{
			for (auto mesh : model->getMeshes())
			{
				auto vb = mesh->compile<VertexBufferFactory, VertexBuffer>();
				m_wireframeGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
			}
		}

		
		for(auto & model : m_model2)
		{
			for (auto mesh : model->getMeshes())
			{
				auto vb = mesh->compile<VertexBufferFactory, VertexBuffer>();
				m_wireframeGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
			}
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
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
