#include "GlfwVulkanWindow.hpp"

GraphicEngine::GLFW::GlfwVulkanWindow::GlfwVulkanWindow(std::shared_ptr<Core::Configuration> cfg, std::shared_ptr<Common::UI> ui) :
	GlfwWindow{ cfg, ui }
{
	this->setWindowGlfwProfile(WindowGlfwProfile::Vulkan);
}
