#pragma once

#include "../../../Common/Widget.hpp"
#include "../../../Core/Subject.hpp"

namespace GraphicEngine::GUI
{
	class TabBarItem : public Widget
	{
	public:
		TabBarItem(std::string label);

		// Inherited via Widget
		virtual void draw() override;
	private:
		std::string label;
	};
}