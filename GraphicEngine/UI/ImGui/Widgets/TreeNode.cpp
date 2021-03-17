#include "TreeNode.hpp"
#include "imgui.h"

GraphicEngine::GUI::TreeNode::TreeNode(std::string label) :
	Widget{ label }
{
}

void GraphicEngine::GUI::TreeNode::draw()
{
	if (ImGui::TreeNode(label.c_str()))
	{
		for (auto children : m_childrens)
		{
			children->draw();
		}
		ImGui::TreePop();
		ImGui::Separator();
	}
}
