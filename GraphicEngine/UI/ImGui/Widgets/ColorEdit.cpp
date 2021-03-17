#include "ColorEdit.hpp"

#include <imgui.h>

GraphicEngine::GUI::ColorEdit::ColorEdit(std::string label, glm::vec4 initialColor):
	Widget{ label }, color { initialColor }
{
}

void GraphicEngine::GUI::ColorEdit::draw()
{
	if (ImGui::ColorEdit4(label.c_str(), &color[0]))
		m_colorSelectorEventListener.notify(color);
}
