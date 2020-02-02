#include "OpenGLRenderingEngine.hpp"

#include <GL/glew.h>

bool GraphicEngine::Vulkan::OpenGLRenderingEngine::drawFrame()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return false;
}

void GraphicEngine::Vulkan::OpenGLRenderingEngine::init(size_t width, size_t height)
{
	glViewport(0, 0, width, height);
}

void GraphicEngine::Vulkan::OpenGLRenderingEngine::resizeFrameBuffer(size_t width, size_t height)
{
	glViewport(0, 0, width, height);
}
