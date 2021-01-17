#pragma once

#include "../Widgets/CollapsingHeader.hpp"
#include "../../../Services/CameraControllerManager.hpp"

namespace GraphicEngine::GUI
{
	class CameraManagerWindow : public Widget
	{
	public:
		CameraManagerWindow(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager);
	protected:
		// Inherited via Widget
		virtual void draw() override;

		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<CollapsingHeader> m_container;
	};
}