#include "Text.hpp"
#include <imgui.h>

GraphicEngine::GUI::Text::Text(std::string label):
	Widget{ label }
{
}

void GraphicEngine::GUI::Text::draw()
{
	ImGui::Text(label.c_str());
}
