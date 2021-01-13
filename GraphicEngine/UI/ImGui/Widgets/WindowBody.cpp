#include "WindowBody.hpp"
#include <imgui.h>

GraphicEngine::GUI::WindowBody::WindowBody(std::string label, bool open):
	label{ label }, open{ open }
{
}

void GraphicEngine::GUI::WindowBody::draw()
{
	if (!ImGui::Begin(label.c_str(), &open))
	{
		ImGui::End();
		return;
	}

	for (auto children : m_childrens)
	{
		children->draw();
	}
	ImGui::End();
}
