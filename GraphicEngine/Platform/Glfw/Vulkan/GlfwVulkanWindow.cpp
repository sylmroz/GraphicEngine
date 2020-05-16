#include "GlfwVulkanWindow.hpp"

GraphicEngine::GLFW::GlfwVulkanWindow::GlfwVulkanWindow(std::shared_ptr<Core::Configuration> cfg):
	GlfwWindow(cfg)
{
	this->setWindowGlfwProfile(WindowGlfwProfile::Vulkan);
}
