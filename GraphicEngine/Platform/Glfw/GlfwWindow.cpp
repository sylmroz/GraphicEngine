#include "GlfwWindow.hpp"
#include <stdexcept>

using namespace GraphicEngine::Core::Inputs;

std::vector<KeyboardKey> GraphicEngine::GLFW::GlfwWindow::getPressedKeys()
{
	_keys.clear();
	for (const auto& [key, value] : m_mappedKeys)
	{
		if (glfwGetKey(m_glfwWindow.get(), key) == GLFW_PRESS)
			_keys.push_back(value);
	}

	return std::move(_keys);
}

std::vector<MouseButton> GraphicEngine::GLFW::GlfwWindow::getPressedButtons()
{
	m_pressedButtons.clear();
	for (const auto& [key, value] : m_mappedButtons)
	{
		if (glfwGetMouseButton(m_glfwWindow.get(), key))
			m_pressedButtons.push_back(value);
	}


	return std::move(m_pressedButtons);
}

void GraphicEngine::GLFW::GlfwWindow::setCursorPosition(const glm::vec2& pos)
{
	glfwSetCursorPos(m_glfwWindow.get(), pos.x, pos.y);
	m_cursorPosition = pos;
}

glm::vec2 GraphicEngine::GLFW::GlfwWindow::getCursorPosition()
{
	return m_cursorPosition;
}

glm::vec2 GraphicEngine::GLFW::GlfwWindow::getScrollValue()
{
	auto scroll = m_scrollValues;
	m_scrollValues = glm::vec2(0.0f);
	return scroll;
}

GraphicEngine::GLFW::GlfwWindow::~GlfwWindow()
{
	glfwTerminate();
}

void GraphicEngine::GLFW::GlfwWindow::swapBuffer()
{
	m_specialApi->swapBuffers(m_glfwWindow.get());
}

void GraphicEngine::GLFW::GlfwWindow::initialize()
{
	glfwInit();

	if (m_windowProfile == WindowGlfwProfile::OpenGL)
	{
		m_specialApi = std::make_shared<WindowGlfwOpenGL>();
	}
	else if (m_windowProfile == WindowGlfwProfile::Vulkan)
	{
		m_specialApi = std::make_shared<WindowGlfwVulkan>();
	}

	m_specialApi->init();

	m_glfwWindow = std::shared_ptr<GLFWwindow>(
		glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr),
		[](GLFWwindow* window)
		{
			glfwDestroyWindow(window);
		});

	glfwMakeContextCurrent(m_glfwWindow.get());

	if (m_glfwWindow == nullptr)
		throw std::runtime_error("Failed to create GLFW window!");

	glfwSetWindowUserPointer(m_glfwWindow.get(), this);

	auto resizeCallback = [](GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		app->m_resizeSubject.notify(width, height);
	};

	auto scrollFun = [](GLFWwindow* window, double xOffset, double yOffset)
	{
		auto app = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		app->m_scrollValues = glm::vec2(xOffset, yOffset);
	};

	auto positionFun = [](GLFWwindow* window, double xPos, double yPos)
	{
		auto app = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		app->m_cursorPosition = glm::vec2(xPos, yPos);
	};

	glfwSetScrollCallback(m_glfwWindow.get(), scrollFun);
	glfwSetCursorPosCallback(m_glfwWindow.get(), positionFun);

	glfwSetFramebufferSizeCallback(m_glfwWindow.get(), resizeCallback);
}

void GraphicEngine::GLFW::GlfwWindow::poolEvents()
{
	glfwPollEvents();
}

bool GraphicEngine::GLFW::GlfwWindow::windowShouldBeClosed()
{
	return glfwWindowShouldClose(m_glfwWindow.get());
}

std::pair<uint32_t, uint32_t> GraphicEngine::GLFW::GlfwWindow::getFrameBufferSize()
{
	int width{ 0 }, height{ 0 };
	glfwGetFramebufferSize(m_glfwWindow.get(), &width, &height);
	return std::pair<uint32_t, uint32_t>(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}

std::shared_ptr<GLFWwindow> GraphicEngine::GLFW::GlfwWindow::getGlfwWindow()
{
	return this->m_glfwWindow;
}

void GraphicEngine::GLFW::GlfwWindow::WindowGlfwOpenGL::init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void GraphicEngine::GLFW::GlfwWindow::WindowGlfwOpenGL::swapBuffers(GLFWwindow* window)
{
	glfwSwapBuffers(window);
}

void GraphicEngine::GLFW::GlfwWindow::WindowGlfwVulkan::init()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void GraphicEngine::GLFW::GlfwWindow::WindowGlfwVulkan::swapBuffers(GLFWwindow* window)
{
	// Do nothing. Vulkan Rendering Engine take care about it
}
