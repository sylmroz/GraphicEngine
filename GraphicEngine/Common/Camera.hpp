#pragma once

#include "../Core/Input/Keyboard/KeyboardEventProxy.hpp"
#include "../Core/Input/Mouse/MouseEnumButton.hpp"

#include "../Core/Configuration.hpp"
#include "../Core/EventManager.hpp"
#include "../Core/Logger.hpp"

#include "WindowKeyboardMouse.hpp"

#include <functional>

#include <glm/glm.hpp>

namespace GraphicEngine::Services
{
	class CameraControllerManager;
}

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
		Camera(std::shared_ptr<Core::Configuration> cfg);

		void rotate(const glm::vec2& offset);
		void move(const glm::vec2& offset);
		void zoom(double offset);

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
		float getFOV();
		void setAspectRatio(float aspectRatio);

		void setCameraType(CameraType cameraType);
		CameraType getCameraType();

		glm::vec3 getPosition();

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

		std::shared_ptr<Core::Configuration> m_cfg;
		std::shared_ptr<Services::CameraControllerManager> m_logger;
	};
}
