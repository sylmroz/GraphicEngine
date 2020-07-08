#include "OpenGLRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"
#include "../../Core/IO/FileSystem.hpp"

#include "OpenGLTextureFactory.hpp"

GraphicEngine::OpenGL::OpenGLRenderingEngine::OpenGLRenderingEngine(
	std::shared_ptr<Common::Camera> camera,
	std::shared_ptr<Core::EventManager> eventManager,
	std::unique_ptr<Core::Logger<OpenGLRenderingEngine>> logger) :
	m_logger(std::move(logger)),
	RenderingEngine(camera, eventManager)
{
	m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Create OpenGL rendering engine instance.");
}

bool GraphicEngine::OpenGL::OpenGLRenderingEngine::drawFrame()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_program->use();
	auto v = m_camera->getViewProjectionMatrix();
	m_uniformBufferMatrix->update(&v);
	m_texture->use(0);
	m_vertexBuffer->bind();
	m_vertexBuffer->draw();
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
		OpenGLVertexShader vert(readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("basicPCTVP.vert").string()));
		OpenGLFragmentShader frag(readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("basicPCTVP.frag").string()));

		m_program = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, frag });
		
		auto uniformIndex = glGetUniformBlockIndex(m_program->getShaderProgramId(), "MVP");
		glUniformBlockBinding(m_program->getShaderProgramId(), uniformIndex, 0);
		auto textureIndex = glGetUniformLocation(m_program->getShaderProgramId(), "texture1");
		glUniform1i(textureIndex, 0);

		m_uniformBufferMatrix = std::make_shared<UniformBuffer<glm::mat4>>();
		m_vertexBuffer = std::make_unique<VertexBuffer<GraphicEngine::Common::VertexPCTc>>(vertices, indices);  //std::unique_ptr<VertexBuffer<GraphicEngine::Common::VertexPCTc>>(new VertexBuffer<GraphicEngine::Common::VertexPCTc>(vertices, indices));
		vertices.clear();
		indices.clear();
		
		m_texture = TextureFactory::produceTexture("C:/rem.png");
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
