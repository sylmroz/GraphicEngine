#include "WindManagerWindow.hpp"

#include "../Widgets/ArrowDirection.hpp"
#include "../Widgets/Text.hpp"
#include "../Widgets/Separator.hpp"
#include "../Widgets/Slider.hpp"
#include "../Widgets/Image.hpp"
#include "../../../Drivers/OpenGL/OpenGLTexture.hpp"

GraphicEngine::GUI::WindManagerWindow::WindManagerWindow(std::shared_ptr<Services::WindManager> windManager):
	m_windManager{ windManager }
{
	m_container = std::make_shared<CollapsingHeader>("Wind manager");
	m_container->addChildren(std::make_shared<Text>("Parameters"));

	auto slider = std::make_shared<SliderFloat>("Speed", windManager->getWindParameters().speed, 0.0, 1.0);
	slider->onSliderChange([windManager](float value) { windManager->setWindSpeed(value); });
	m_container->addChildren(slider);

	auto windArrow = std::make_shared<ArrowDirection2D>("Wind direction", windManager->getWindParameters().direction);
	windArrow->subscribe([windManager](glm::vec2 value) { windManager->setWindDirection(value); });
	m_container->addChildren(windArrow);
}

void GraphicEngine::GUI::WindManagerWindow::init()
{
	m_container->addChildren(std::make_shared<Separator>());

	m_container->addChildren(std::make_shared<Text>("Texture"));
	auto texture = m_windManager->getTextureObject<OpenGL::Texture2D>();
	auto windTexture = std::make_shared<Image>((texture->getTexture()), texture->getWidth(), texture->getHeight());
	m_container->addChildren(windTexture);
}

void GraphicEngine::GUI::WindManagerWindow::draw()
{
	m_container->draw();
}
