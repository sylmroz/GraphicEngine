#include "GlfwVulkanWindow.hpp"

GraphicEngine::GLFW::GlfwVulkanWindow::GlfwVulkanWindow()
{
	this->setWindowGlfwProfile(WindowGlfwProfile::Vulkan);
}
