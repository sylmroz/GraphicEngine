#pragma once

#include "../Widgets/Checkbox.hpp"
#include "../Widgets/CollapsingHeader.hpp"
#include "../Widgets/ColorEdit.hpp"
#include "../../../Services/RenderingOptionsManager.hpp"

namespace GraphicEngine::GUI
{
	class RenderingSettingsWindow : public Widget
	{
	public:
		RenderingSettingsWindow(std::shared_ptr<Services::RenderingOptionsManager> renderingOptionsManager);
	protected:
		// Inherited via Widget
		virtual void draw() override;
		std::shared_ptr<CollapsingHeader> m_container;
		std::shared_ptr<Services::RenderingOptionsManager> m_renderingOptionsManager;
	};
}