#pragma once

#include "../../../Common/Widget.hpp"
#include "../../../Core/Subject.hpp"

namespace GraphicEngine::GUI
{
	class TabBar : public Widget
	{
	public:
		TabBar(std::string label);

		// Inherited via Widget
		virtual void draw() override;
	};
}