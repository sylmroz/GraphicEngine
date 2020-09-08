#include "OpenGLRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"
#include "../../Core/IO/FileSystem.hpp"

#include "OpenGLTextureFactory.hpp"
#include "OpenGLVertexBufferFactory.hpp"

GraphicEngine::OpenGL::OpenGLRenderingEngine::OpenGLRenderingEngine(
	std::shared_ptr<Common::Camera> camera,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Core::Configuration> cfg,
	std::unique_ptr<Core::Logger<OpenGLRenderingEngine>> logger) :
	m_logger(std::move(logger)),
	RenderingEngine(camera, eventManager, cfg)
{
	m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Create OpenGL rendering engine instance.");
}

bool GraphicEngine::OpenGL::OpenGLRenderingEngine::drawFrame()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_program->use();
	auto v = m_camera->getViewProjectionMatrix();
	m_uniformBufferMatrix->update(&v);

	light.eyePosition = m_camera->getPosition();
	m_lightUniformBuffer->update(&light);
	auto m = m_models.front()->getModelMatrix();
	m_modelMatrix->update(&m);

	for (auto& vbs : m_vertexBuffers)
	{
		for (auto& vb : vbs)
		{
			vb->bind();
			vb->draw();
		}
	}
	m = glm::identity<glm::mat4>();
	m_modelMatrix->update(&m);

	for (auto& vbs : m_vertexBuffers)
	{
		for (auto& vb : vbs)
		{
			vb->bind();
			vb->draw();
		}
	}

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
		/*OpenGLVertexShader vert(readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("basicPCTVP.vert").string()));
		OpenGLFragmentShader frag(readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("basicPCTVP.frag").string()));*/

		OpenGLVertexShader vert(readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("diffuse.vert").string()));
		OpenGLFragmentShader frag(readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("diffuse.frag").string()));

		m_program = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });
		
		auto uniformIndex = glGetUniformBlockIndex(m_program->getShaderProgramId(), "MVP");
		glUniformBlockBinding(m_program->getShaderProgramId(), uniformIndex, 0);
		/*auto textureIndex = glGetUniformLocation(m_program->getShaderProgramId(), "texture1");
		glUniform1i(textureIndex, 0);*/

		m_uniformBufferMatrix = std::make_shared<UniformBuffer<glm::mat4>>(0);
		m_lightUniformBuffer = std::make_shared<UniformBuffer<Light>>(1);
		m_modelMatrix = std::make_shared<UniformBuffer<glm::mat4>>(2);
		//m_vertexBuffer = m_mesh->compile<VertexBufferFactory<Common::VertexPCTc>, VertexBuffer<Common::VertexPCTc>>();
		for (auto& model : m_models)
		{
			m_vertexBuffers.push_back(model->compile<VertexBufferFactory, VertexBuffer>());
		}
		
		//m_texture = TextureFactory::produceTexture("C:/rem.png");

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
