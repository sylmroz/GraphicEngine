#include "TabBar.hpp"
#include <imgui.h>

GraphicEngine::GUI::TabBar::TabBar(std::string label) :
	label{ label }
{
}

void GraphicEngine::GUI::TabBar::draw()
{
	if (ImGui::BeginTabBar(label.c_str(), ImGuiTabBarFlags_None))
	{
		for (auto& children : m_childrens)
			children->draw();
		ImGui::EndTabBar();
	}
}
