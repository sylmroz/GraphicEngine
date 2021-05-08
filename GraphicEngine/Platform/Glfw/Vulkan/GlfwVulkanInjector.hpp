#pragma once
#define BOOST_DI_CFG_DIAGNOSTICS_LEVEL 2
#define __has_builtin(...) 1
#define BOOST_DI_CFG_CTOR_LIMIT_SIZE 20
#include <boost/di.hpp>

#include "../../../Drivers/Vulkan/VulkanRenderingEngine.hpp"
#include "GlfwVulkanWindow.hpp"
#include "GlfwVulkanWindowContext.hpp"


namespace di = boost::di;

namespace GraphicEngine::GLFW
{
	auto injectGlfwVulkanResources()
	{
		return di::make_injector
		(
			di::bind<GraphicEngine::Common::WindowKeyboardMouse, GraphicEngine::GLFW::GlfwWindow>.to<GraphicEngine::GLFW::GlfwVulkanWindow>().in(di::singleton),
			di::bind<GraphicEngine::Vulkan::VulkanWindowContext>.to<GraphicEngine::GLFW::GlfwVulkanWindowContext>().in(di::singleton),
			di::bind<GraphicEngine::RenderingEngine>.to<GraphicEngine::Vulkan::VulkanRenderingEngine>().in(di::singleton),
			di::bind<GraphicEngine::Common::UI>.to<GraphicEngine::GUI::ImGuiImpl>().in(di::singleton),
			di::bind<GraphicEngine::Core::EventManager>.in(di::singleton),
			di::bind<GraphicEngine::Core::Timer>.in(di::singleton)
		);
	}
}
