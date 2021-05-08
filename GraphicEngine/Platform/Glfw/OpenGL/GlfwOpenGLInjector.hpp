#pragma once
#define BOOST_DI_CFG_DIAGNOSTICS_LEVEL 2
#define __has_builtin(...) 1
#define BOOST_DI_CFG_CTOR_LIMIT_SIZE 20
#include <boost/di.hpp>

#include "../../../Drivers/OpenGL/OpenGLRenderingEngine.hpp"
#include "GlfwOpenGLWindow.hpp"

namespace di = boost::di;

namespace GraphicEngine::GLFW
{
	auto injectGlfwOpenGlResources()
	{
		return di::make_injector
		(
			di::bind<GraphicEngine::Common::WindowKeyboardMouse, GraphicEngine::GLFW::GlfwWindow>.to<GraphicEngine::GLFW::GlfwOpenGLWindow>().in(di::singleton),
			di::bind<GraphicEngine::RenderingEngine>.to<GraphicEngine::OpenGL::OpenGLRenderingEngine>().in(di::unique),
			di::bind<GraphicEngine::Common::UI>.to<GraphicEngine::GUI::ImGuiImpl>().in(di::singleton),
			di::bind<GraphicEngine::Core::EventManager>.in(di::singleton),
			di::bind<GraphicEngine::Services::WindManager>.in(di::singleton)
		);
	}
}
