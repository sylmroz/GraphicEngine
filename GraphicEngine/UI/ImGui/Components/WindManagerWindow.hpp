#pragma once

#include "../Widgets/Checkbox.hpp"
#include "../Widgets/CollapsingHeader.hpp"
#include "../Widgets/ColorEdit.hpp"
#include "../../../Services/WindManager.hpp"

namespace GraphicEngine::GUI
{
	class WindManagerWindow : public Widget
	{
	public:
		WindManagerWindow(std::shared_ptr<Services::WindManager> windManager);
		virtual void init() override;
	protected:
		// Inherited via Widget
		virtual void draw() override;

		std::shared_ptr<Services::WindManager> m_windManager;
		std::shared_ptr<CollapsingHeader> m_container;
	};
}