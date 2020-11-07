#include "CameraController.hpp"

GraphicEngine::Common::CameraController::CameraController(std::shared_ptr<Camera> camera, std::shared_ptr<WindowKeyboardMouse> window,
	std::shared_ptr<Core::EventManager> eventManager, std::shared_ptr<Core::Inputs::KeyboardEventProxy> keyboard,
	std::shared_ptr<Core::Timer> timer, std::shared_ptr<Core::Logger<Services::CameraControllerManager>> logger) :
	m_camera{ camera },
	m_window{ window },
	m_eventManager{ eventManager },
	m_keyboard{ keyboard },
	m_timer{ timer },
	m_logger{ logger },
	m_prevMousePosition(glm::vec2(0.0f, 0.0f))
{
	setInitialMousePosition(glm::vec2(m_window->getWidth() / 2, m_window->getHeight() / 2));
	updateCamera(
		m_window->getCursorPosition(),
		m_window->getScrollValue(),
		m_window->getPressedButtons(),
		m_window->getPressedKeys()
	);

	m_camera->setSensitivity(m_window->getSensitivity());

	m_window->addResizeCallbackListener([&](size_t width, size_t height)
	{
		if (width == 0 || height == 0)
			return;
		m_camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
	});

	m_timer->onTimeUpdate([&](double dt)
	{
		setDt(dt);
	});

	m_keyboard->onKeyUp([&](Core::Inputs::KeyboardKey key)
	{
		if (key == Core::Inputs::KeyboardKey::KEY_C)
		{
			if (isCameraActive())
				switchCameraType();
		}
	});

	m_eventManager->addSubject([&]()
	{
		if (isCameraActive())
			updateCamera(
				m_window->getCursorPosition(),
				m_window->getScrollValue(),
				m_window->getPressedButtons(),
				m_window->getPressedKeys()
			);
	});
}


void GraphicEngine::Common::CameraController::setCameraType(CameraType cameraType)
{
	m_camera->setCameraType(cameraType);
}

void GraphicEngine::Common::CameraController::setDt(float dt)
{
	m_dt = dt;
}

void GraphicEngine::Common::CameraController::setInitialMousePosition(glm::vec2 pos)
{
	m_prevMousePosition = pos;
}

void GraphicEngine::Common::CameraController::updateCamera(glm::vec2 cursorPosition, glm::vec2 scrollPosition, const std::vector<Core::Inputs::MouseButton>& buttons, std::vector<Core::Inputs::KeyboardKey> keys)
{
	//m_logger->info(__FILE__, __LINE__, __FUNCTION__, "zoom offset x:{} y:{}", scrollPosition.x, scrollPosition.y);
	move(keys);
	zoom(-scrollPosition.y);
	rotate(cursorPosition, buttons);
}

bool GraphicEngine::Common::CameraController::isCameraActive()
{
	return isActive;
}

void GraphicEngine::Common::CameraController::rotate(glm::vec2 pos, const std::vector<Core::Inputs::MouseButton>& buttons)
{
	glm::vec2 newOffset = m_prevMousePosition - pos;

	if (m_rotateButton == Core::Inputs::MouseButton::buttonNone || std::find(std::begin(buttons), std::end(buttons), m_rotateButton) != std::end(buttons))
	{
		m_camera->rotate(newOffset);
	}

	m_prevMousePosition = pos;
}

void GraphicEngine::Common::CameraController::move(std::vector<Core::Inputs::KeyboardKey> keys)
{
	using namespace Core::Inputs;
	std::vector<KeyboardKey> filteredKeys = GraphicEngine::Core::Ranges::filter(keys, [&](KeyboardKey key)
	{
		return std::find(std::begin(m_basicMovementKeys), std::end(m_basicMovementKeys), key) != std::end(m_basicMovementKeys);
	});

	glm::vec2 movementOffset{ 0.0,0.0 };
	for (KeyboardKey key : filteredKeys)
	{
		if (key == KeyboardKey::KEY_W)
			movementOffset.x += m_dt;
		else if (key == KeyboardKey::KEY_S)
			movementOffset.x -= m_dt;
		else if (key == KeyboardKey::KEY_A)
			movementOffset.y -= m_dt;
		else if (key == KeyboardKey::KEY_D)
			movementOffset.y += m_dt;
	}

	m_camera->move(movementOffset);
}

void GraphicEngine::Common::CameraController::zoom(double offset)
{
	m_camera->zoom(offset);
}

std::shared_ptr<GraphicEngine::Common::Camera> GraphicEngine::Common::CameraController::getCamera()
{
	return m_camera;
}

void GraphicEngine::Common::CameraController::switchCameraType()
{
	auto camType = m_camera->getCameraType();
	camType == CameraType::Perspective ?
		m_camera->setCameraType(CameraType::Orthographic) :
		m_camera->setCameraType(CameraType::Perspective);
}

void GraphicEngine::Common::CameraController::activate()
{
	isActive = true;
}

void GraphicEngine::Common::CameraController::deactivate()
{
	isActive = false;
}
