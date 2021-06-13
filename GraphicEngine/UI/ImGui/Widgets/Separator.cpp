#include "Separator.hpp"
#include <imgui.h>

GraphicEngine::GUI::Separator::Separator():
	Widget{ "" }
{
}

void GraphicEngine::GUI::Separator::draw()
{
	ImGui::Separator();
}
