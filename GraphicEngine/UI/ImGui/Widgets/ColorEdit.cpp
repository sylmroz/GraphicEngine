#include "ColorEdit.hpp"

#include <imgui.h>

GraphicEngine::GUI::ColorEdit::ColorEdit(glm::vec4 initialColor):
	color{ initialColor }
{
}

void GraphicEngine::GUI::ColorEdit::draw()
{
	ImGui::ColorEdit3("color", &color[0]);
	m_colorSelectorEventListener.notify(color);
}
