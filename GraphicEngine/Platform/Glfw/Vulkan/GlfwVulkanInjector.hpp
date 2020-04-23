#pragma once

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
			di::bind<GraphicEngine::Common::CameraController>.in(di::singleton),
			di::bind<GraphicEngine::Common::Camera>.in(di::singleton),
			di::bind<GraphicEngine::Core::EventManager>.in(di::singleton)
		);
	}
}
