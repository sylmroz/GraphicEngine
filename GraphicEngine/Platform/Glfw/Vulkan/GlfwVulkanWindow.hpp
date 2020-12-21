#pragma once

#define GLFW_INCLUDE_VULKAN

#include "../GlfwWindow.hpp"

namespace GraphicEngine::GLFW
{
	class GlfwVulkanWindow : public GlfwWindow
	{
	public:
		GlfwVulkanWindow(std::shared_ptr<Core::Configuration> cfg, std::shared_ptr<Common::UI> ui);
	};
}
