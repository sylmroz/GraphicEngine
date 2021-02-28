#include "Checkbox.hpp"
#include <imgui.h>

GraphicEngine::GUI::Checkbox::Checkbox(std::string label, bool initialChecked):
	label{ label }, checked{ initialChecked }
{
}

void GraphicEngine::GUI::Checkbox::draw()
{
	if (ImGui::Checkbox(label.c_str() , &checked))
		m_checkboxEventListener.notify(checked);
}
