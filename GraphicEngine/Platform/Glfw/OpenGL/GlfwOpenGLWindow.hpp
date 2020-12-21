#pragma once

#include "../GlfwWindow.hpp"

namespace GraphicEngine::GLFW
{
	class GlfwOpenGLWindow : public GlfwWindow
	{
	public:
		GlfwOpenGLWindow(std::shared_ptr<Core::Configuration> cfg, std::shared_ptr<Common::UI> ui);
	};
}
