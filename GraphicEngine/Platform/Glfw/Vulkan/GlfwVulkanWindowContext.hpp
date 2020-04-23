#pragma once
#include "GlfwVulkanWindow.hpp"
#include "../../../Drivers/Vulkan/VulkanWindowContext.hpp"

namespace GraphicEngine::GLFW
{
	class GlfwVulkanWindowContext : public Vulkan::VulkanWindowContext
	{
	public:
		GlfwVulkanWindowContext(std::shared_ptr<GlfwWindow> glfwWindow);
		VkSurfaceKHR createSurface(const vk::UniqueInstance& instance) override;
		std::vector<std::string> getRequiredExtensions() override;

	protected:
		std::shared_ptr<GlfwWindow> _glfwWindow;
	};
}
