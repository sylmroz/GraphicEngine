#ifndef GRAPHIC_ENGINE_COMMMON_CAMERA_HPP
#define GRAPHIC_ENGINE_COMMMON_CAMERA_HPP

#include <functional>

#include <glm/glm.hpp>

namespace GraphicEngine::Commmon
{
	enum class CameraType
	{
		Perspective,
		Orthogonal
	};

	class Camera
	{
	public:

		Camera();
		Camera(float fov, float aspectRatio, float zNear, float zFar);

		void rotate(const glm::vec2& offset);
		void move(const glm::vec3& offset);

		glm::mat4 getViewProjectionMatrix();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		void setCameraPerspectiveProperties(float fov, float aspectRatio, float zNear, float zFar);
		void setCameraOrthogonalProperties(float left, float right, float top, float bottom);

		void setSpeed(float speed);
		float getSpeed();
		void setSensitivity(float sensitivity);
		float getSensitivity();
		void setFOV(float fov);
		void setAspectRatio(float aspectRatio);

		void setCameraType(CameraType cameraType);

	private:
		glm::mat4 caclulatePerspective();
		glm::mat4 calculateOrthogonal();

		void updateViewMatrix();
		void updateProjectionMatrix();

		std::function<glm::mat4(void)> calculateProjectionMatrix = [&]()->glm::mat4 
		{
			return caclulatePerspective();
		};
	
	private:
		glm::mat4 _viewMatrix;
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewProjectionMatrix;

		glm::vec3 _position = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 _direction = glm::vec3(0.0, 0.0, 1.0);
		glm::vec3 _up = glm::vec3(0.0, 1.0, 0.0);

		glm::vec2 _yawPitch = glm::vec2(0.0, 0.0);
		glm::vec2 _yawPitchOffset = glm::vec2(0.0, 0.0);

		// Perspective matrix
		float _fov{ 45.0f };
		float _aspectRatio{ 1920.0f / 1080.0f }; // Typical full hd resolution
		float _zNear{ 0.001f };
		float _zFar{ 100.0f };

		// Orthogonal matrix
		float _left{ 10.0f };
		float _right{ 10.0f };
		float _bottom{ 10.0f };
		float _top{ 10.0f };

	private:
		bool _shouldUpdateView{ false };
		bool _shouldUpdateProjection{ false };
		CameraType _cameraType = CameraType::Perspective;
		float _speed{ 1.0f };
		float _sensitivity{ 1.0f };
	};
}

#endif // !GRAPHIC_ENGINE_COMMMON_CAMERA_HPP

