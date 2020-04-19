#include "GlfwVulkanWindowContext.hpp"

GraphicEngine::GLFW::GlfwVulkanWindowContext::GlfwVulkanWindowContext(std::shared_ptr<GlfwWindow> glfwWindow) :
	_glfwWindow(glfwWindow)
{
}

VkSurfaceKHR GraphicEngine::GLFW::GlfwVulkanWindowContext::createSurface(const vk::UniqueInstance& instance)
{
	VkSurfaceKHR _surface;
	glfwCreateWindowSurface(instance.get(), _glfwWindow->getGlfwWindow().get(), nullptr, &_surface);

	return _surface;
}

std::vector<std::string> GraphicEngine::GLFW::GlfwVulkanWindowContext::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<std::string> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	return extensions;
}