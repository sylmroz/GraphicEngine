#pragma once

#include "../Core/Input/Keyboard/KeyboardEnumKeys.hpp"
#include "../Core/Input/Mouse/MouseEnumButton.hpp"

#include "../Core/EventManager.hpp"
#include "WindowKeyboardMouse.hpp"

#include <functional>

#include <glm/glm.hpp>

#include <boost/di.hpp>


namespace di = boost::di;

inline auto PerspectiveParameters = []() {};

namespace GraphicEngine::Common
{
	enum class CameraType
	{
		Perspective,
		Orthogonal
	};

	struct PerspectiveParameters_s
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
		BOOST_DI_INJECT(Camera, (named = PerspectiveParameters) PerspectiveParameters_s perspectiveParameters);
		Camera(OrthogonalParameters ororthogonalParameter);

		void rotate(const glm::vec2& offset);
		void move(const glm::vec2& offset);

		glm::mat4 getViewProjectionMatrix();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		void setCameraPerspectiveProperties(PerspectiveParameters_s perspectiveParameters);
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
		glm::mat4 m_viewMatrix = glm::mat4();
		glm::mat4 m_projectionMatrix = glm::mat4();
		glm::mat4 m_viewProjectionMatrix = glm::mat4();

		glm::vec3 m_position = glm::vec3(2.0, 2.0, 2.0);
		glm::vec3 m_direction = glm::normalize(glm::vec3(-1.0, -1.0, -1.0));
		glm::vec3 m_up = glm::vec3(0.0, 0.0, 1.0);

		glm::vec2 m_yawPitch = glm::vec2(0.0, 0.0);
		glm::vec2 m_yawPitchOffset = glm::vec2(0.0, 0.0);

		PerspectiveParameters_s m_perspectiveParameters;

		OrthogonalParameters m_orthogonalParameters;

	private:
		bool m_shouldUpdateView{ true };
		bool m_shouldUpdateProjection{ true };
		CameraType m_cameraType = CameraType::Perspective;
		float m_speed{ 1.0f };
		float m_sensitivity{ 180.0f };
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

	private:
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<WindowKeyboardMouse> m_window;
		std::shared_ptr<Core::EventManager> m_eventManager;

		float m_dt{ 0.0f };
		glm::vec2 m_prevMousePosition;
		Core::Inputs::MouseButton m_rotateButton = Core::Inputs::MouseButton::buttonLeft;
	};
}

