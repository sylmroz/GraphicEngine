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

void GraphicEngine::Commmon::Camera::setCameraPerspectiveProperties(float fov, float aspectRatio, float zNear, float zFar)
{
	_fov = fov;
	_aspectRatio = aspectRatio;
	_zNear = zNear;
	_zFar = zFar;
}

void GraphicEngine::Commmon::Camera::setCameraOrthogonalProperties(float left, float right, float top, float bottom)
{
	_left = left;
	_right = right;
	_top = top;
	_bottom = bottom;
}

void GraphicEngine::Commmon::Camera::setSpeed(float speed)
{
	_speed = speed;
}

float GraphicEngine::Commmon::Camera::getSpeed()
{
	return _speed;
}

void GraphicEngine::Commmon::Camera::setSensitivity(float sensitivity)
{
	_sensitivity = sensitivity;
}

float GraphicEngine::Commmon::Camera::getSensitivity()
{
	return _sensitivity;
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

GraphicEngine::Commmon::Camera::Camera()
{
}

GraphicEngine::Commmon::Camera::Camera(float fov, float aspectRatio, float zNear, float zFar) :
	_fov(fov), _aspectRatio(glm::radians(aspectRatio)), _zNear(zNear), _zFar(zFar), _cameraType(CameraType::Perspective)
{
}

void GraphicEngine::Commmon::Camera::rotate(const glm::vec2& offset)
{
	_yawPitchOffset = (offset * _sensitivity);
	float oldPitch = _yawPitch.y;
	_yawPitch += _yawPitchOffset;
	
	if (_yawPitch.x > 360.0f || _yawPitch.x < -360.0)
		_yawPitch.x = 0.0f;

	if (_yawPitch.y > 89.0f || _yawPitch.y < -89.0)
	{
		_yawPitch.y = oldPitch;
		_yawPitchOffset.y = 0.0f;
	}

	_shouldUpdateView = true;
}

void GraphicEngine::Commmon::Camera::move(const glm::vec3& offset)
{
	_position = _position + (offset * _speed);
	_shouldUpdateView = true;
}

glm::mat4 GraphicEngine::Commmon::Camera::caclulatePerspective()
{
	return glm::perspective(glm::radians(_fov), _aspectRatio, _zNear, _zFar);
}

glm::mat4 GraphicEngine::Commmon::Camera::calculateOrthogonal()
{
	return glm::ortho(_left, _right, _bottom, _top);
}

void GraphicEngine::Commmon::Camera::updateViewMatrix()
{
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 right = glm::normalize(glm::cross(_direction, up));
	glm::quat yawQuat = glm::angleAxis(glm::radians(_yawPitchOffset.x), up);
	glm::quat pitchQuat = glm::angleAxis(glm::radians(_yawPitchOffset.y), right);
	glm::quat rot = glm::normalize(glm::cross(yawQuat, pitchQuat));
	_direction = glm::normalize(glm::rotate(rot, _direction));
	_up = glm::normalize(glm::cross(right, _direction));

	_viewMatrix = glm::lookAt(_position, _position + _direction, _up);

	_yawPitchOffset = glm::vec2(0.0f, 0.0f);
	_shouldUpdateView = false;
}

void GraphicEngine::Commmon::Camera::updateProjectionMatrix()
{
	_projectionMatrix = calculateProjectionMatrix();
	_shouldUpdateProjection = false;
}
