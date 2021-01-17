#include "TabBarItem.hpp"
#include <imgui.h>

GraphicEngine::GUI::TabBarItem::TabBarItem(std::string label):
	label{ label }
{
}

void GraphicEngine::GUI::TabBarItem::draw()
{
	if (ImGui::BeginTabItem(label.c_str()))
	{
		for (auto& children : m_childrens)
			children->draw();
		ImGui::EndTabItem();
	}
}
