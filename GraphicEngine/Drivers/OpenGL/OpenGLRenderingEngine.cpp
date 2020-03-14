#include "OpenGLRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"

GraphicEngine::OpenGL::OpenGLRenderingEngine::OpenGLRenderingEngine(std::shared_ptr<Window> window):
	RenderingEngine(window)
{
}

bool GraphicEngine::OpenGL::OpenGLRenderingEngine::drawFrame()
{
	glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	program->use();
	vertexBuffer->bind();
	vertexBuffer->draw();
	return false;
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::init(size_t width, size_t height)
{
	using namespace GraphicEngine::Core::IO;
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("OpenGL initialization failed!");
	}
	glViewport(0, 0, width, height);

	OpenGLVertexShader vert(readFile<std::string>("C:/Projects/GraphicEngine/GraphicEngine/Assets/Shaders/Glsl/basic.vert"));
	OpenGLFragmentShader frag(readFile<std::string>("C:/Projects/GraphicEngine/GraphicEngine/Assets/Shaders/Glsl/basic.frag"));
	program = std::unique_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram({ vert, frag }));
	vertexBuffer = std::unique_ptr<VertexBuffer<GraphicEngine::Common::VertexP>>(new VertexBuffer<GraphicEngine::Common::VertexP>(vertices, indices));
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::resizeFrameBuffer(size_t width, size_t height)
{
	glViewport(0, 0, width, height);
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::cleanup()
{
}
