#include "Button.hpp"

#include <imgui.h>

GraphicEngine::GUI::Button::Button(std::string label):
	Widget{ label }
{
}

void GraphicEngine::GUI::Button::draw()
{
	if (ImGui::Button(label.c_str()))
        m_clickedSubject.notify();
}

void GraphicEngine::GUI::Button::onClicked(std::function<void(void)> callback)
{
	m_clickedSubject.subscribe(callback);
}
