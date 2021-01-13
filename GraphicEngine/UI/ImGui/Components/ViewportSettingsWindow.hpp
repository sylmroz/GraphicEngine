#pragma once

#include "../Widgets/Checkbox.hpp"
#include "../Widgets/CollapsingHeader.hpp"
#include "../Widgets/ColorEdit.hpp"
#include "../../../Services/ViewportManager.hpp"

namespace GraphicEngine::GUI
{
	class ViewportSettingWindow : public Widget
	{
	public:
		ViewportSettingWindow(std::shared_ptr<Services::ViewportManager> viewportManager);
	protected:
		// Inherited via Widget
		virtual void draw() override;

		std::shared_ptr<Services::ViewportManager> m_viewportManager;
		std::shared_ptr<CollapsingHeader> m_container;
	};
}