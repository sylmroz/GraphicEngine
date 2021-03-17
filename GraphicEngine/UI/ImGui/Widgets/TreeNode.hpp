#pragma once

#include "../../../Common/Widget.hpp"

namespace GraphicEngine::GUI
{
	class TreeNode : public Widget
	{
	public:
		TreeNode(std::string label);
		// Inherited via Widget
		virtual void draw() override;
	};
}
