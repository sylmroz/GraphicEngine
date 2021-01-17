#include "InputFloat.hpp"
#include <imgui.h>

GraphicEngine::GUI::InputScalar4::InputScalar4(std::string label, glm::vec4 initialValue, float step) :
	value{ initialValue }, step{ step }, label{ label }
{
}

void GraphicEngine::GUI::InputScalar4::draw()
{
	if (ImGui::InputScalarN(label.c_str(), ImGuiDataType_Float, &value[0], 4, &step))
		m_inputScalarSubject.notify(value);
}

void GraphicEngine::GUI::InputScalar4::onInputScalarEdit(std::function<void(glm::vec4)> callback)
{
	m_inputScalarSubject.subscribe(callback);
}

GraphicEngine::GUI::InputScalar::InputScalar(std::string label, float initialValue, float step) :
	value{ initialValue }, step{ step }, label{ label }
{
}

void GraphicEngine::GUI::InputScalar::draw()
{
	if (ImGui::InputScalar(label.c_str(), ImGuiDataType_Float, &value, &step))
		m_inputScalarSubject.notify(value);
}

void GraphicEngine::GUI::InputScalar::onInputScalarEdit(std::function<void(float)> callback)
{
	m_inputScalarSubject.subscribe(callback);
}
