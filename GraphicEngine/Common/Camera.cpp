#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

glm::mat4 GraphicEngine::Commmon::Camera::getViewProjectionMatrix()
{
	return _viewProjectionMatrix;
}

glm::mat4 GraphicEngine::Commmon::Camera::getViewMatrix()
{
	if (_shouldUpdateView)
		updateViewMatrix();
	return _viewMatrix;
}

glm::mat4 GraphicEngine::Commmon::Camera::getProjectionMatrix()
{
	if (_shouldUpdateProjection)
		updateProjectionMatrix();
	return _projectionMatrix;
}

void GraphicEngine::Commmon::Camera::setSpeed(float speed)
{
	_speed = speed;
}

void GraphicEngine::Commmon::Camera::setFOV(float fov)
{
	_fov = fov;
	_shouldUpdateProjection = true;
}

void GraphicEngine::Commmon::Camera::setAspectRatio(float aspectRatio)
{
	_aspectRatio = aspectRatio;
	_shouldUpdateProjection = _shouldUpdateProjection;
}

void GraphicEngine::Commmon::Camera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
	_shouldUpdateProjection = true;
}

void GraphicEngine::Commmon::Camera::rotate(const glm::vec2& offset)
{
	_yawPitch = _yawPitch + (offset * _speed);
	_shouldUpdateView = true;
}

void GraphicEngine::Commmon::Camera::move(const glm::vec3& offset)
{
	_position = _position + (offset * _speed);
	_shouldUpdateView = true;
}

glm::mat4 GraphicEngine::Commmon::Camera::caclulatePerspective()
{
	return glm::perspective(_fov, _aspectRatio, _zNear, _zFar);
}

glm::mat4 GraphicEngine::Commmon::Camera::calculateOrthogonal()
{
	return glm::ortho(_left, _right, _bottom, _top);
}

void GraphicEngine::Commmon::Camera::updateViewMatrix()
{
	_viewMatrix = glm::lookAt(_position, _position + _direction, _up);
	_shouldUpdateView = false;
}

void GraphicEngine::Commmon::Camera::updateProjectionMatrix()
{
	_projectionMatrix = calculateProjectionMatrix();
	_shouldUpdateProjection = false;
}
