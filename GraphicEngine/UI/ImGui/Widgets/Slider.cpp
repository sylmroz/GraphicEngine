#include "Slider.hpp"
#include <imgui.h>

void GraphicEngine::GUI::SliderFloat::draw()
{
	if (ImGui::SliderFloat(label.c_str(), &value, m_minValue, m_maxValue))
	{
		m_sliderValueChange.notify(value);
	}
}

void GraphicEngine::GUI::SliderFloat2::draw()
{
	if (ImGui::SliderFloat2(label.c_str(), &value[0], m_minValue, m_maxValue))
	{
		m_sliderValueChange.notify(value);
	}
}

void GraphicEngine::GUI::SliderFloat3::draw()
{
	if (ImGui::SliderFloat3(label.c_str(), &value[0], m_minValue, m_maxValue))
	{
		m_sliderValueChange.notify(value);
	}
}

void GraphicEngine::GUI::SliderFloat4::draw()
{
	if (ImGui::SliderFloat3(label.c_str(), &value[0], m_minValue, m_maxValue))
	{
		m_sliderValueChange.notify(value);
	}
}
