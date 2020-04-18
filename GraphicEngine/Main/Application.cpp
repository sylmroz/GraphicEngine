#include "Application.hpp"
#include "Engine.hpp"
#include "../Drivers/Vulkan/VulkanRenderingEngine.hpp"
#include "../Platform/Glfw/Vulkan/GlfwVulkanWindow.hpp"
#include "../Platform/Glfw/OpenGL/GlfwOpenGLWindow.hpp"
#include "../Common/RenderingEngine.hpp"
#include "../Common/WindowKeyboardMouse.hpp"

#include <exception>
#include <iostream>

#include <boost/di.hpp>


namespace di = boost::di;

using namespace GraphicEngine::Core::Inputs;

Application::Application(int argc, char** argv)
{
	// TODO - parse argc and argv to get parameters

}

void Application::exec()
{
	try
	{
		GraphicEngine::Common::PerspectiveParameters_s perspectiveParameters;

		auto injector = di::make_injector(
			//di::bind<GraphicEngine::GLFW::GlfwVulkanWindow>.in(di::singleton),
			//di::bind<GraphicEngine::GLFW::GlfwWindow>.to<GraphicEngine::GLFW::GlfwVulkanWindow>().in(di::singleton),
			di::bind<GraphicEngine::Common::WindowKeyboardMouse, GraphicEngine::GLFW::GlfwWindow>.to<GraphicEngine::GLFW::GlfwVulkanWindow>().in(di::singleton),
			di::bind<GraphicEngine::Vulkan::VulkanWindowContext>.to<GraphicEngine::GLFW::GlfwVulkanWindowContext>().in(di::singleton),
			di::bind<GraphicEngine::Common::CameraController>.in(di::singleton),
			di::bind<GraphicEngine::Common::Camera>.in(di::singleton),
			di::bind<GraphicEngine::Common::PerspectiveParameters_s>.named(PerspectiveParameters).to(perspectiveParameters),
			di::bind<GraphicEngine::RenderingEngine>.to<GraphicEngine::Vulkan::VulkanRenderingEngine>().in(di::singleton)
		);

		auto engine = injector.create<std::shared_ptr<GraphicEngine::Engine>>();
		engine->initialize();
		engine->run();
	}

	catch (std::exception ex)
	{
		std::cout << ex.what() << "\n";
	}
}
