#pragma once

#include "../../../Common/Widget.hpp"

namespace GraphicEngine::GUI
{
	class Text : public Widget
	{
	public:
		Text(std::string label);

		// Inherited via Widget
		virtual void draw() override;
	private:
	};
}