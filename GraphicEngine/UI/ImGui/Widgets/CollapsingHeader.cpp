#include "CollapsingHeader.hpp"
#include <imgui.h>

GraphicEngine::GUI::CollapsingHeader::CollapsingHeader(std::string label):
	label{ label }
{
}

void GraphicEngine::GUI::CollapsingHeader::draw()
{
	if (ImGui::CollapsingHeader(label.c_str()))
	{
		for (auto children : m_childrens)
		{
			children->draw();
		}
	}
}
