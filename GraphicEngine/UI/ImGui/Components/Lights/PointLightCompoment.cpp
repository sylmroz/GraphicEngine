#include "PointLightCompoment.hpp"

GraphicEngine::GUI::PointLightComponent::PointLightComponent() :
	LightComponent<Engines::Graphic::Shaders::PointLight>{ "Point"}
{
	m_positionInput = std::make_shared<InputScalar4>("Direction", currentLightParameters.position, 0.05f);
	m_positionInput->onInputScalarEdit([&](glm::vec4 value)
	{
		currentLightParameters.position = value;
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});

	m_constantInput = std::make_shared<InputScalar>("Constant", currentLightParameters.constant, 0.05f);
	m_constantInput->onInputScalarEdit([&](float value)
	{
		currentLightParameters.constant = value;
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});

	m_linearInput = std::make_shared<InputScalar>("Linear", currentLightParameters.linear, 0.05f);
	m_linearInput->onInputScalarEdit([&](float value)
	{
		currentLightParameters.linear = value;
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});

	m_quadrictInput = std::make_shared<InputScalar>("Quadric", currentLightParameters.quadric, 0.05f);
	m_quadrictInput->onInputScalarEdit([&](float value)
	{
		currentLightParameters.quadric = value;
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});
}

void GraphicEngine::GUI::PointLightComponent::drawEditTools()
{
	m_positionInput->draw();
	m_constantInput->draw();
	m_linearInput->draw();
	m_quadrictInput->draw();
}

void GraphicEngine::GUI::PointLightComponent::updateRestComponents()
{
	m_positionInput->value = currentLightParameters.position;
	m_constantInput->value = currentLightParameters.constant;
	m_linearInput->value = currentLightParameters.linear;
	m_quadrictInput->value = currentLightParameters.quadric;
}
