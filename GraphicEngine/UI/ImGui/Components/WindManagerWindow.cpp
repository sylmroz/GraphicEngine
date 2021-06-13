#include "WindManagerWindow.hpp"

#include "../Widgets/Text.hpp"
#include "../Widgets/Separator.hpp"
#include "../Widgets/Slider.hpp"

GraphicEngine::GUI::WindManagerWindow::WindManagerWindow(std::shared_ptr<Services::WindManager> windManager):
	m_windManager{ windManager }
{
	m_container = std::make_shared<CollapsingHeader>("Wind manager");
	m_container->addChildren(std::make_shared<Text>("Parameters"));

	auto slider = std::make_shared<SliderFloat>("Speed", windManager->getWindParameters().speed, 0.0, 1.0);
	slider->onSliderChange([windManager](float value) { windManager->setWindSpeed(value); });
	m_container->addChildren(slider);

	m_container->addChildren(std::make_shared<Separator>());
}

void GraphicEngine::GUI::WindManagerWindow::draw()
{
	m_container->draw();
}
