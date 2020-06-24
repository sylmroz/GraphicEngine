#pragma once

#include "../Core/Input/Keyboard/KeyboardEnumKeys.hpp"
#include "../Core/Input/Mouse/MouseEnumButton.hpp"

#include "../Core/EventManager.hpp"
#include "WindowKeyboardMouse.hpp"

#include <functional>

#include <glm/glm.hpp>

inline auto PerspectiveParameters = []() {};

namespace GraphicEngine::Common
{
	enum class CameraType
	{
		Perspective,
		Orthographic
	};

	struct CameraParameters
	{
		float fov{ 45.0f };
		float aspectRatio{ 1920.0f / 1080.0f }; // Typical full hd resolution
		float zNear{ 0.001f };
		float zFar{ 100.0f };
	};


	class Camera
	{
	public:
		Camera();

		void rotate(const glm::vec2& offset);
		void move(const glm::vec2& offset);

		glm::mat4 getViewProjectionMatrix();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		void setCameraPerspectiveProperties(CameraParameters cameraParameters);
		void setCameraOrthographicProperties(CameraParameters cameraParameters);

		void setSpeed(float speed);
		float getSpeed();
		void setSensitivity(float sensitivity);
		float getSensitivity();
		void setFOV(float fov);
		void setAspectRatio(float aspectRatio);

		void setCameraType(CameraType cameraType);
		CameraType getCameraType();

	private:
		glm::mat4 caclulatePerspective();
		glm::mat4 calculateOrthographic();

		void updateViewMatrix();
		void updateProjectionMatrix();

		std::function<glm::mat4(void)> calculateProjectionMatrix;

	private:
		glm::mat4 m_viewMatrix = glm::mat4();
		glm::mat4 m_projectionMatrix = glm::mat4();
		glm::mat4 m_viewProjectionMatrix = glm::mat4();

		glm::vec3 m_position = glm::vec3(2.0, 2.0, 2.0);
		glm::vec2 m_positionOffset = m_position;
		glm::vec3 m_direction = glm::normalize(glm::vec3(-1.0, -1.0, -1.0));
		glm::vec3 m_new_direction = m_direction;
		glm::vec3 m_up = glm::vec3(0.0, 0.0, 1.0);
		
		glm::vec2 m_yawPitch = glm::vec2(0.0, 0.0);
		glm::vec2 m_yawPitchOffset = glm::vec2(0.0, 0.0);

		CameraParameters m_cameraParameters;

	private:
		bool m_shouldUpdateView{ true };
		bool m_shouldUpdateProjection{ true };
		CameraType m_cameraType = CameraType::Perspective;
		float m_speed{ 1.0f };
		float m_sensitivity{ 0.25f };

		std::map<CameraType, std::function<glm::mat4()>> m_projectionMatrixCalculators
		{
			{CameraType::Perspective, [&]()->glm::mat4 { return caclulatePerspective(); }},
			{CameraType::Orthographic, [&]()->glm::mat4 { return calculateOrthographic(); }}
		};
	};

	class CameraController
	{
	public:
		CameraController(std::shared_ptr<Camera> camera, std::shared_ptr<WindowKeyboardMouse> window, std::shared_ptr<Core::EventManager> eventManager);

		void setCameraType(CameraType cameraType);

		void setDt(float dt);

		void setInitialMousePosition(glm::vec2 pos);

		bool isCameraActivated();

		void updateCamera(glm::vec2 cursorPosition, glm::vec2 scrollPosition,
			const std::vector<Core::Inputs::MouseButton>& buttons,
			std::vector<Core::Inputs::KeyboardKey> keys);

		void rotate(glm::vec2 pos, const std::vector<GraphicEngine::Core::Inputs::MouseButton>& buttons);

		void move(std::vector<Core::Inputs::KeyboardKey> keys);

		void zoom(double offset);

		std::shared_ptr<Camera> getCamera();

		void switchCameraType();

	private:
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<WindowKeyboardMouse> m_window;
		std::shared_ptr<Core::EventManager> m_eventManager;

		float m_dt{ 0.0f };
		glm::vec2 m_prevMousePosition;
		Core::Inputs::MouseButton m_rotateButton = Core::Inputs::MouseButton::buttonLeft;
	};
}
