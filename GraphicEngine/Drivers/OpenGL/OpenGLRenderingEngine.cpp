#include "OpenGLRenderingEngine.hpp"

#include <GL/glew.h>

GraphicEngine::OpenGL::OpenGLRenderingEngine::OpenGLRenderingEngine(std::shared_ptr<Window> window):
	RenderingEngine(window)
{
}

bool GraphicEngine::OpenGL::OpenGLRenderingEngine::drawFrame()
{
	glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return false;
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::init(size_t width, size_t height)
{
	glViewport(0, 0, width, height);
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::resizeFrameBuffer(size_t width, size_t height)
{
	glViewport(0, 0, width, height);
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::cleanup()
{
}
