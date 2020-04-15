#include "GlfwWindow.hpp"
#include <stdexcept>

using namespace GraphicEngine::Core::Inputs;

std::vector<KeyboardKey> GraphicEngine::GLFW::GlfwWindow::getPressedKeys()
{
	_keys.clear();
	if (glfwGetKey(_glfwWindow.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		_keys.push_back(KeyboardKey::KEY_ESCAPE);
	if (glfwGetKey(_glfwWindow.get(), GLFW_KEY_W) == GLFW_PRESS)
		_keys.push_back(KeyboardKey::KEY_W);
	if (glfwGetKey(_glfwWindow.get(), GLFW_KEY_A) == GLFW_PRESS)
		_keys.push_back(KeyboardKey::KEY_A);
	if (glfwGetKey(_glfwWindow.get(), GLFW_KEY_S) == GLFW_PRESS)
		_keys.push_back(KeyboardKey::KEY_S);
	if (glfwGetKey(_glfwWindow.get(), GLFW_KEY_D) == GLFW_PRESS)
		_keys.push_back(KeyboardKey::KEY_D);

	// Implement more if needed
	return std::move(_keys);
}

std::vector<MouseButton> GraphicEngine::GLFW::GlfwWindow::getPressedButtons()
{
	_pressedButtons.clear();
	if (glfwGetMouseButton(_glfwWindow.get(), GLFW_MOUSE_BUTTON_LEFT))
		_pressedButtons.push_back(MouseButton::buttonLeft);
	if (glfwGetMouseButton(_glfwWindow.get(), GLFW_MOUSE_BUTTON_MIDDLE))
		_pressedButtons.push_back(MouseButton::butonMiddle);
	if (glfwGetMouseButton(_glfwWindow.get(), GLFW_MOUSE_BUTTON_RIGHT))
		_pressedButtons.push_back(MouseButton::buttonRight);

	return std::move(_pressedButtons);
}

void GraphicEngine::GLFW::GlfwWindow::setCursorPosition(const glm::vec2& pos)
{
	glfwSetCursorPos(_glfwWindow.get(), pos.x, pos.y);
	_cursorPosition = pos;
}

glm::vec2 GraphicEngine::GLFW::GlfwWindow::getCursorPosition()
{
	return _cursorPosition;
}

glm::vec2 GraphicEngine::GLFW::GlfwWindow::getScrollValue()
{
	return _scrollValues;
}

GraphicEngine::GLFW::GlfwWindow::~GlfwWindow()
{
	glfwTerminate();
}

void GraphicEngine::GLFW::GlfwWindow::swapBuffer()
{
	_specialApi->swapBuffers(_glfwWindow.get());
}

void GraphicEngine::GLFW::GlfwWindow::initialize()
{
	glfwInit();
	
	if (_windowProfile == GLFWWindowProfile::OpenGL)
	{
		_specialApi = std::shared_ptr<WindowGLFWApi>(new WindowGLFWOpenGl);
	}
	else if (_windowProfile == GLFWWindowProfile::Vulkan)
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

	glfwMakeContextCurrent(_glfwWindow.get());

	if (_glfwWindow == nullptr)
		throw std::runtime_error("Failed to create GLFW window!");

	glfwSetWindowUserPointer(_glfwWindow.get(), this);

	auto resizeCallback = [](GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		app->_resizeSubject.notify(width, height);
	};

	auto scrollFun = [](GLFWwindow* window, double xOffset, double yOffset)
	{
		auto app = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		app->_scrollValues = glm::vec2(xOffset, yOffset);
	};

	auto positionFun = [](GLFWwindow* window, double xPos, double yPos)
	{
		auto app = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		app->_cursorPosition = glm::vec2(xPos, yPos);
	};

	glfwSetScrollCallback(_glfwWindow.get(), scrollFun);
	glfwSetCursorPosCallback(_glfwWindow.get(), positionFun);

	glfwSetFramebufferSizeCallback(_glfwWindow.get(), resizeCallback);
}

void GraphicEngine::GLFW::GlfwWindow::poolEvents()
{
	glfwPollEvents();
}

bool GraphicEngine::GLFW::GlfwWindow::windowShouldBeClosed()
{
	return glfwWindowShouldClose(_glfwWindow.get());
}

std::pair<uint32_t, uint32_t> GraphicEngine::GLFW::GlfwWindow::getFrameBufferSize()
{
	int width{ 0 }, height{ 0 };
	glfwGetFramebufferSize(_glfwWindow.get(), &width, &height);
	return std::pair<uint32_t, uint32_t>(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}

std::shared_ptr<GLFWwindow> GraphicEngine::GLFW::GlfwWindow::getGlfwWindow()
{
	return this->_glfwWindow;
}

void GraphicEngine::GLFW::GlfwWindow::WindowGLFWOpenGl::init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void GraphicEngine::GLFW::GlfwWindow::WindowGLFWOpenGl::swapBuffers(GLFWwindow* window)
{
	glfwSwapBuffers(window);
}

void GraphicEngine::GLFW::GlfwWindow::WindowGLFWVulkan::init()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void GraphicEngine::GLFW::GlfwWindow::WindowGLFWVulkan::swapBuffers(GLFWwindow* window)
{
	// Do nothing. Vulkan Rendering Engine take care about it
}
