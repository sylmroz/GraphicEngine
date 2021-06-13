#pragma once

#include "../../../Common/Widget.hpp"

namespace GraphicEngine::GUI
{
	class Separator : public Widget
	{
	public:
		Separator();

		// Inherited via Widget
		virtual void draw() override;
	private:
	};
}