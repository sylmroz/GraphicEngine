#pragma once

#include "../Widgets/WindowBody.hpp"
#include "CameraManagerWindow.hpp"
#include "LightManagerWindow.hpp"
#include "ViewportSettingsWindow.hpp"
#include "RenderingSettingsWindow.hpp"

namespace GraphicEngine::GUI
{
	class SettingWindow : public Widget
	{
	public:
		SettingWindow(std::shared_ptr<ViewportSettingWindow> settingWindow, std::shared_ptr<RenderingSettingsWindow> renderingSettingWindow, std::shared_ptr<CameraManagerWindow> cameraManagerWindow, std::shared_ptr<LightManagerWindow> lightManager);
	protected:
		// Inherited via Widget
		virtual void draw() override;

		std::shared_ptr<WindowBody> m_body;
	};
}
