#pragma once

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
			di::bind<GraphicEngine::Common::CameraController>.in(di::singleton),
			di::bind<GraphicEngine::Common::Camera>.in(di::singleton),
			di::bind<GraphicEngine::Core::EventManager>.in(di::singleton)
		);
	}
}
