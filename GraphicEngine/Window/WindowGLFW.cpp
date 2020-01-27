#include "WindowGLFW.hpp"

void GraphicEngine::GLFW::WindowGLFW::swapBuffer()
{
}

void GraphicEngine::GLFW::WindowGLFW::initialize()
{
	glfwInit();
	
	if (_windowProfile == OPENGL)
		WindowGLFWOpenGl{}.init();
	else if (_windowProfile == VULKAN)
		WindowGLFWVulkan{}.init();

	_glfwWindow = std::shared_ptr<GLFWwindow>(
		glfwCreateWindow(_width, _height, "Graphic Engine", nullptr, nullptr), 
		[](GLFWwindow* window) {glfwDestroyWindow(window); }
	);


}

void GraphicEngine::GLFW::WindowGLFW::poolEvents()
{
	glfwPollEvents();
}

bool GraphicEngine::GLFW::WindowGLFW::windowShouldBeClosed()
{
	return glfwWindowShouldClose(_glfwWindow.get());
}

void GraphicEngine::GLFW::WindowGLFW::WindowGLFWOpenGl::init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void GraphicEngine::GLFW::WindowGLFW::WindowGLFWVulkan::init()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}
