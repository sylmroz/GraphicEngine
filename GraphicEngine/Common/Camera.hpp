#ifndef GRAPHIC_ENGINE_COMMON_CAMERA_HPP
#define GRAPHIC_ENGINE_COMMON_CAMERA_HPP

#include "../Core/Input/Keyboard/KeyboardEnumKeys.hpp"
#include "../Core/Input/Mouse/MouseEnumButton.hpp"

#include <functional>

#include <glm/glm.hpp>

namespace GraphicEngine::Common
{
	enum class CameraType
	{
		Perspective,
		Orthogonal
	};

	struct PerspectiveParameters
	{
		float fov{ 45.0f };
		float aspectRatio{ 1920.0f / 1080.0f }; // Typical full hd resolution
		float zNear{ 0.001f };
		float zFar{ 100.0f };
	};

	struct OrthogonalParameters
	{
		float left{ 10.0f };
		float right{ 10.0f };
		float bottom{ 10.0f };
		float top{ 10.0f };
	};


	class Camera
	{
	public:

		Camera();
		Camera(PerspectiveParameters perspectiveParameters);
		Camera(OrthogonalParameters ororthogonalParameter);

		void rotate(const glm::vec2& offset);
		void move(const glm::vec2& offset);

		glm::mat4 getViewProjectionMatrix();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		void setCameraPerspectiveProperties(PerspectiveParameters perspectiveParameters);
		void setCameraOrthogonalProperties(OrthogonalParameters orthogonalParameters);

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
		glm::mat4 _viewMatrix = glm::mat4();
		glm::mat4 _projectionMatrix = glm::mat4();
		glm::mat4 _viewProjectionMatrix = glm::mat4();

		glm::vec3 _position = glm::vec3(2.0, 2.0, 2.0);
		glm::vec3 _direction = glm::normalize(glm::vec3(-1.0, -1.0, -1.0));
		glm::vec3 _up = glm::vec3(0.0, 0.0, 1.0);

		glm::vec2 _yawPitch = glm::vec2(0.0, 0.0);
		glm::vec2 _yawPitchOffset = glm::vec2(0.0, 0.0);

		PerspectiveParameters _perspectiveParameters;

		OrthogonalParameters _orthogonalParameters;

	private:
		bool _shouldUpdateView{ true };
		bool _shouldUpdateProjection{ true };
		CameraType _cameraType = CameraType::Perspective;
		float _speed{ 0.5f };
		float _sensitivity{ 1.0f };
	};

	class CameraController
	{
	public:
		CameraController(std::shared_ptr<Camera> camera);

		void setCameraType(CameraType cameraType);

		void setDt(float dt);

		void setInitialMousePosition(float x, float y);

		void rotate(float x, float y, const std::vector<GraphicEngine::Core::Inputs::MouseButton>& buttons);

		void move(std::vector<GraphicEngine::Core::Inputs::KeyboardKey> keys);

	private:
		std::shared_ptr<Camera> _camera;
		float _dt{ 0.0f };
		glm::vec2 _prevMousePosition;
		GraphicEngine::Core::Inputs::MouseButton _rotateButton = GraphicEngine::Core::Inputs::MouseButton::buttonLeft;
	};
}

#endif // !GRAPHIC_ENGINE_COMMON_CAMERA_HPP

