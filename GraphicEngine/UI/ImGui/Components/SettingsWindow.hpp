#pragma once

#include "../Widgets/WindowBody.hpp"
#include "ViewportSettingsWindow.hpp"

namespace GraphicEngine::GUI
{
	class SettingWindow : public Widget
	{
	public:
		SettingWindow(std::shared_ptr<ViewportSettingWindow> settingWindow);
	protected:
		// Inherited via Widget
		virtual void draw() override;

		std::shared_ptr<WindowBody> m_body;
	};
}
