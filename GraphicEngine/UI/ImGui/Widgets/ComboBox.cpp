#include "ComboBox.hpp"
#include <imgui.h>

GraphicEngine::GUI::ComboBox::ComboBox(std::string label, std::vector<std::string> labels, int selectedItemIndex):
	Widget{ label },
	labels{ labels },
	selectedItemIndex{ selectedItemIndex }
{
}

void GraphicEngine::GUI::ComboBox::draw()
{
	std::vector<const char*> tmpLabels;
	for (auto& lab : labels)
	{
		tmpLabels.push_back(lab.c_str());
	}
	ImGui::Combo(label.c_str(), &selectedItemIndex, &tmpLabels[0], labels.size());
	m_selectedItemEventListener.notify(selectedItemIndex);
}

void GraphicEngine::GUI::ComboBox::setLabels(std::vector<std::string> labels)
{
	this->labels = labels;
}

void GraphicEngine::GUI::ComboBox::onItemSelected(std::function<void(int)> callback)
{
	m_selectedItemEventListener.subscribe(callback);
}
