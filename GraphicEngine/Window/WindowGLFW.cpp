#include "WindowGLFW.hpp"
#include <stdexcept>

using namespace GraphicEngine::HID;

GraphicEngine::GLFW::WindowGLFW::~WindowGLFW()
{
	glfwTerminate();
}

void GraphicEngine::GLFW::WindowGLFW::swapBuffer()
{
	_specialApi->swapBuffers(_glfwWindow.get());
}

void GraphicEngine::GLFW::WindowGLFW::initialize()
{
	glfwInit();
	

	if (_windowProfile == OPENGL)
	{
		_specialApi = std::shared_ptr<WindowGLFWApi>(new WindowGLFWOpenGl);
	}
	else if (_windowProfile == VULKAN)
	{
		_specialApi = std::shared_ptr<WindowGLFWApi>(new WindowGLFWVulkan);
	}

	_specialApi->init();

	_glfwWindow = std::shared_ptr<GLFWwindow>(
		glfwCreateWindow(_width, _height, "Graphic Engine", nullptr, nullptr), 
		[](GLFWwindow* window) 
		{
			glfwDestroyWindow(window);
		});

	if (_glfwWindow == nullptr)
		throw std::runtime_error("Failed to create GLFW window!");

	glfwSetWindowUserPointer(_glfwWindow.get(), this);

	auto resizeCallback = [](GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
		app->_resizeSubject.next(width, height);
	};

	glfwSetFramebufferSizeCallback(_glfwWindow.get(), resizeCallback);
}

void GraphicEngine::GLFW::WindowGLFW::poolEvents()
{
	glfwPollEvents();
	grabAllKeys();
	grabAllPressedMouseButtons();
}

bool GraphicEngine::GLFW::WindowGLFW::windowShouldBeClosed()
{
	return glfwWindowShouldClose(_glfwWindow.get());
}

void GraphicEngine::GLFW::WindowGLFW::registerMouse(std::shared_ptr<GraphicEngine::HID::Mouse> mouse)
{
	_mouse = mouse;

	auto scrollFun = [](GLFWwindow* window, double xOffset, double yOffset)
	{
		auto app = reinterpret_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
		app->_mouse->scrollnEventHandlerNotify(xOffset, yOffset);
	};

	auto positionlFun = [](GLFWwindow* window, double xPos, double ypos)
	{
		auto app = reinterpret_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
		app->_mouse->positionEventHandlerNotify(xPos, ypos);
	};

	glfwSetScrollCallback(_glfwWindow.get(), scrollFun);
	glfwSetCursorPosCallback(_glfwWindow.get(), positionlFun);
}

void GraphicEngine::GLFW::WindowGLFW::grabAllKeys()
{
	
	std::vector<Key> keys;
	if (glfwGetKey(_glfwWindow.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		keys.push_back(Key::KEY_ESCAPE);
	if (glfwGetKey(_glfwWindow.get(), GLFW_KEY_W) == GLFW_PRESS)
		keys.push_back(Key::KEY_W);
	if (glfwGetKey(_glfwWindow.get(), GLFW_KEY_A) == GLFW_PRESS)
		keys.push_back(Key::KEY_A);
	if (glfwGetKey(_glfwWindow.get(), GLFW_KEY_S) == GLFW_PRESS)
		keys.push_back(Key::KEY_S);
	if (glfwGetKey(_glfwWindow.get(), GLFW_KEY_D) == GLFW_PRESS)
		keys.push_back(Key::KEY_D);

	// Implement more if needed
	if (!keys.empty())
		_keyboard->notify(std::move(keys));
}

void GraphicEngine::GLFW::WindowGLFW::grabAllPressedMouseButtons()
{
	std::vector<Button> buttons;

	if (glfwGetMouseButton(_glfwWindow.get(), GLFW_MOUSE_BUTTON_LEFT))
		buttons.push_back(Button::BUTTON_LEFT);
	if (glfwGetMouseButton(_glfwWindow.get(), GLFW_MOUSE_BUTTON_MIDDLE))
		buttons.push_back(Button::BUTTON_MIDDLE);
	if (glfwGetMouseButton(_glfwWindow.get(), GLFW_MOUSE_BUTTON_RIGHT))
		buttons.push_back(Button::BUTTON_RIGHT);

	if (!buttons.empty())
		_mouse->notify(std::move(buttons));
}

void GraphicEngine::GLFW::WindowGLFW::WindowGLFWOpenGl::init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void GraphicEngine::GLFW::WindowGLFW::WindowGLFWOpenGl::swapBuffers(GLFWwindow* window)
{
	glfwSwapBuffers(window);
}

void GraphicEngine::GLFW::WindowGLFW::WindowGLFWVulkan::init()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void GraphicEngine::GLFW::WindowGLFW::WindowGLFWVulkan::swapBuffers(GLFWwindow* window)
{
	// Do nothing. Vulkan Rendering Engine take care about it
}
