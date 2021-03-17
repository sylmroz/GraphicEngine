#pragma once

#include "../../../Common/Widget.hpp"

namespace GraphicEngine::GUI
{
	class CollapsingHeader : public Widget
	{
	public:
		CollapsingHeader(std::string label);
		// Inherited via Widget
		virtual void draw() override;
	};
}