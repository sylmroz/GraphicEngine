#include "Camera.hpp"
#include "../Core/Ranges.hpp"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

glm::mat4 GraphicEngine::Common::Camera::getViewProjectionMatrix()
{
	return getProjectionMatrix() * getViewMatrix();
}

glm::mat4 GraphicEngine::Common::Camera::getViewMatrix()
{
	if (m_shouldUpdateView)
		updateViewMatrix();
	return m_viewMatrix;
}

glm::mat4 GraphicEngine::Common::Camera::getProjectionMatrix()
{
	if (m_shouldUpdateProjection)
		updateProjectionMatrix();
	return m_projectionMatrix;
}

void GraphicEngine::Common::Camera::setCameraPerspectiveProperties(PerspectiveParameters_s perspectiveParameters)
{
	m_perspectiveParameters = perspectiveParameters;
	m_cameraType = CameraType::Perspective;
	m_shouldUpdateProjection = true;
}

void GraphicEngine::Common::Camera::setCameraOrthogonalProperties(OrthogonalParameters orthogonalParameters)
{
	m_orthogonalParameters = orthogonalParameters;
	m_cameraType = CameraType::Orthogonal;
	m_shouldUpdateProjection = true;
}

void GraphicEngine::Common::Camera::setSpeed(float speed)
{
	m_speed = speed;
}

float GraphicEngine::Common::Camera::getSpeed()
{
	return m_speed;
}

void GraphicEngine::Common::Camera::setSensitivity(float sensitivity)
{
	m_sensitivity = sensitivity;
}

float GraphicEngine::Common::Camera::getSensitivity()
{
	return m_sensitivity;
}

void GraphicEngine::Common::Camera::setFOV(float fov)
{
	m_perspectiveParameters.fov = fov;
	m_shouldUpdateProjection = true;
}

void GraphicEngine::Common::Camera::setAspectRatio(float aspectRatio)
{
	m_perspectiveParameters.aspectRatio = aspectRatio;
	m_shouldUpdateProjection = true;
}

void GraphicEngine::Common::Camera::setCameraType(CameraType cameraType)
{
	m_cameraType = cameraType;
	m_shouldUpdateProjection = true;
}

GraphicEngine::Common::Camera::Camera()
{
	PerspectiveParameters_s perspectiveParameters;
	setCameraPerspectiveProperties(perspectiveParameters);
}

GraphicEngine::Common::Camera::Camera(PerspectiveParameters_s perspectiveParameters)
{
	setCameraPerspectiveProperties(perspectiveParameters);
}

GraphicEngine::Common::Camera::Camera(OrthogonalParameters orthogonalParameters)
{
	setCameraOrthogonalProperties(orthogonalParameters);
}

void GraphicEngine::Common::Camera::rotate(const glm::vec2& offset)
{
	m_yawPitchOffset = (offset * m_sensitivity);
	float oldPitch = m_yawPitch.y;
	m_yawPitch += m_yawPitchOffset;

	if (m_yawPitch.x > 360.0f || m_yawPitch.x < -360.0)
		m_yawPitch.x = 0.0f;

	if (m_yawPitch.y > 89.0f || m_yawPitch.y < -89.0)
	{
		m_yawPitch.y = oldPitch;
		m_yawPitchOffset.y = 0.0f;
	}
	m_shouldUpdateView = true;
}

void GraphicEngine::Common::Camera::move(const glm::vec2& offset)
{
	if (offset.x != 0)
		m_position = m_position + (m_direction * offset.x * m_speed);
	if (offset.y != 0)
		m_position = m_position + (glm::normalize(glm::cross(m_direction, glm::vec3(0.0f, 0.0f, 1.0f))) * offset.y * m_speed);
	m_shouldUpdateView = true;
}

glm::mat4 GraphicEngine::Common::Camera::caclulatePerspective()
{
	return glm::perspective(glm::radians(m_perspectiveParameters.fov), m_perspectiveParameters.aspectRatio, m_perspectiveParameters.zNear, m_perspectiveParameters.zFar);
}

glm::mat4 GraphicEngine::Common::Camera::calculateOrthogonal()
{
	return glm::ortho(m_orthogonalParameters.left, m_orthogonalParameters.right, m_orthogonalParameters.bottom, m_orthogonalParameters.top);
}

void GraphicEngine::Common::Camera::updateViewMatrix()
{
	glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 right = glm::normalize(glm::cross(m_direction, up));
	glm::quat yawQuat = glm::angleAxis(glm::radians(m_yawPitchOffset.x), up);
	glm::quat pitchQuat = glm::angleAxis(glm::radians(m_yawPitchOffset.y), right);
	if (yawQuat != glm::quat(1.0, 0.0, 0.0, 0.0) && pitchQuat != glm::quat(1.0, 0.0, 0.0, 0.0))
	{
		glm::quat rot = glm::normalize(glm::cross(yawQuat, pitchQuat));
		m_direction = glm::normalize(glm::rotate(rot, m_direction));
		m_up = glm::normalize(glm::cross(right, m_direction));
	}

	m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);

	m_yawPitchOffset = glm::vec2(0.0f, 0.0f);
	m_shouldUpdateView = false;
}

void GraphicEngine::Common::Camera::updateProjectionMatrix()
{
	m_projectionMatrix = calculateProjectionMatrix();
	m_shouldUpdateProjection = false;
}

GraphicEngine::Common::CameraController::CameraController(std::shared_ptr<Camera> camera, std::shared_ptr<WindowKeyboardMouse> window, std::shared_ptr<Core::EventManager> eventManager) :
	m_camera(camera),
	m_window(window),
	m_eventManager(eventManager)
{
	m_eventManager->addSubject([&]()
		{
			if (isCameraActivated())
			{
				updateCamera(
					m_window->getCursorPosition(),
					m_window->getScrollValue(),
					m_window->getPressedButtons(),
					m_window->getPressedKeys()
				);
				//_window->setCursorPosition(glm::vec2(_window->getWidth() / 2, _window->getHeight() / 2));
			}
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
	move(keys);
	zoom(scrollPosition.x);
	rotate(cursorPosition, buttons);
}

bool GraphicEngine::Common::CameraController::isCameraActivated()
{
	// TODO
	return true;
}

void GraphicEngine::Common::CameraController::rotate(glm::vec2 pos, const std::vector<Core::Inputs::MouseButton>& buttons)
{
	glm::vec2 newOffset = m_prevMousePosition - pos;

	if (m_rotateButton == Core::Inputs::MouseButton::buttonNone || std::find(std::begin(buttons), std::end(buttons), m_rotateButton) != std::end(buttons))
	{
		m_camera->rotate(newOffset * m_dt);
	}

	m_prevMousePosition = pos;
}

void GraphicEngine::Common::CameraController::move(std::vector<Core::Inputs::KeyboardKey> keys)
{
	using namespace Core::Inputs;
	std::vector<KeyboardKey> basicMovementKeys{ KeyboardKey::KEY_W, KeyboardKey::KEY_A, KeyboardKey::KEY_S, KeyboardKey::KEY_D };
	std::vector<KeyboardKey> filteredKeys = GameEngine::Core::Ranges::filter(keys, [&](KeyboardKey key)
		{
			return std::find(std::begin(basicMovementKeys), std::end(basicMovementKeys), key) != std::end(basicMovementKeys);
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
}
