#pragma once

#include "../../../Common/Widget.hpp"

namespace GraphicEngine::GUI
{
	class WindowBody : public Widget
	{
	public:
		WindowBody(std::string label, bool open = true);
		// Inherited via Widget
		virtual void draw() override;
	private:
		std::string label;
		bool open;
	};
}
