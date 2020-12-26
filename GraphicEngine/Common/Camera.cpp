#include "Camera.hpp"
#include "../Core/Ranges.hpp"
#include "../Core/Utils/ObjectConverter.hpp"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

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

void GraphicEngine::Common::Camera::setCameraPerspectiveProperties(CameraParameters cameraParameters)
{
	m_cameraParameters = cameraParameters;
	setCameraType(CameraType::Perspective);
}

void GraphicEngine::Common::Camera::setCameraOrthographicProperties(CameraParameters cameraParameters)
{
	m_cameraParameters = cameraParameters;
	setCameraType(CameraType::Orthographic);
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
	if (fov > 0 && fov <= 90)
	{
		m_cameraParameters.fov = fov;
		m_shouldUpdateProjection = true;
	}
}

float GraphicEngine::Common::Camera::getFOV()
{
	return m_cameraParameters.fov;
}

void GraphicEngine::Common::Camera::setAspectRatio(float aspectRatio)
{
	m_cameraParameters.aspectRatio = aspectRatio;
	m_shouldUpdateProjection = true;
}

void GraphicEngine::Common::Camera::setCameraType(CameraType cameraType)
{
	m_cameraType = cameraType;
	m_shouldUpdateProjection = true;
	calculateProjectionMatrix = m_projectionMatrixCalculators[m_cameraType];
}

GraphicEngine::Common::CameraType GraphicEngine::Common::Camera::getCameraType()
{
	return m_cameraType;
}

glm::vec3 GraphicEngine::Common::Camera::getPosition()
{
	return m_position;
}

GraphicEngine::Common::Camera::Camera(std::shared_ptr<Core::Configuration> cfg) :
	m_cfg{ cfg }
{
	CameraParameters param
	{
		m_cfg->getProperty<float>("parameters:fov"),
		m_cfg->getProperty<float>("parameters:aspect ratio"),
		m_cfg->getProperty<float>("parameters:z-near"),
		m_cfg->getProperty<float>("parameters:z-far"),
	};

	if (m_cfg->getProperty<std::string>("type") == "perspective")
	{
		setCameraPerspectiveProperties(param);
	}

	else
	{
		setCameraOrthographicProperties(param);
	}

	m_position = Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("position"));
	m_direction = glm::normalize(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("direction")));
	m_up = Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("up"));
	m_speed = m_cfg->getProperty<float>("speed");
	rotate(glm::vec2(0.001));
}

void GraphicEngine::Common::Camera::rotate(const glm::vec2& offset)
{
	m_yawPitchOffset = (offset * m_sensitivity);
	float oldPitch = m_yawPitch.x;
	m_yawPitch += m_yawPitchOffset;

	m_shouldUpdateView = true;
}

void GraphicEngine::Common::Camera::move(const glm::vec2& offset)
{
	if (offset.x != 0)
	{
		m_position = m_position + (m_new_direction * offset.x * m_speed);
	}
	if (offset.y != 0)
	{
		m_position = m_position + (glm::normalize(glm::cross(m_new_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * offset.y * m_speed);
	}

	m_positionOffset -= offset;
	m_shouldUpdateView = true;
}

void GraphicEngine::Common::Camera::zoom(double offset)
{
	setFOV(getFOV() + offset);
}

glm::mat4 GraphicEngine::Common::Camera::caclulatePerspective()
{
	return glm::perspective(glm::radians(m_cameraParameters.fov), m_cameraParameters.aspectRatio,
		m_cameraParameters.zNear, m_cameraParameters.zFar);
}

glm::mat4 GraphicEngine::Common::Camera::calculateOrthographic()
{
	float orthoBoundary = m_positionOffset.x * glm::radians(m_cameraParameters.fov);
	return glm::ortho(-orthoBoundary * m_cameraParameters.aspectRatio, orthoBoundary * m_cameraParameters.aspectRatio,
		-orthoBoundary, orthoBoundary,
		-m_cameraParameters.zFar, m_cameraParameters.zFar);
}

void GraphicEngine::Common::Camera::updateViewMatrix()
{
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0,0.0,-1.0), up));
	glm::quat yawQuat = glm::angleAxis(glm::radians(m_yawPitch.x), up);
	glm::quat pitchQuat = glm::angleAxis(glm::radians(m_yawPitch.y), right);
	if (yawQuat != glm::quat(1.0, 0.0, 0.0, 0.0) && pitchQuat != glm::quat(1.0, 0.0, 0.0, 0.0))
	{
		glm::quat rot = glm::normalize(glm::cross(yawQuat, pitchQuat));
		m_new_direction = glm::normalize(glm::rotate(rot, m_direction));
	}
	m_up = glm::normalize(glm::cross(glm::normalize(glm::cross(m_new_direction, up)), m_new_direction));
	m_viewMatrix = glm::lookAt(m_position, m_position + m_new_direction, m_up);

	m_yawPitchOffset = glm::vec2(0.0f, 0.0f);
	m_shouldUpdateView = false;
	if (m_cameraType == CameraType::Orthographic)
	{
		m_shouldUpdateProjection = true;
	}
}

void GraphicEngine::Common::Camera::updateProjectionMatrix()
{
	m_projectionMatrix = calculateProjectionMatrix();
	m_shouldUpdateProjection = false;
}
