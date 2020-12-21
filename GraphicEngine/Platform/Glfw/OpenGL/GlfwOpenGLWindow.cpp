#include "GlfwOpenGLWindow.hpp"

GraphicEngine::GLFW::GlfwOpenGLWindow::GlfwOpenGLWindow(std::shared_ptr<Core::Configuration> cfg, std::shared_ptr<Common::UI> ui):
	GlfwWindow{ cfg, ui }
{
	setWindowGlfwProfile(WindowGlfwProfile::OpenGL);
}
