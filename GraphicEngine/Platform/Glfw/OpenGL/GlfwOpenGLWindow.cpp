#include "GlfwOpenGLWindow.hpp"

GraphicEngine::GLFW::GlfwOpenGLWindow::GlfwOpenGLWindow(std::shared_ptr<Core::Configuration> cfg):
GlfwWindow(cfg)
{
	setWindowGlfwProfile(WindowGlfwProfile::OpenGL);
}
