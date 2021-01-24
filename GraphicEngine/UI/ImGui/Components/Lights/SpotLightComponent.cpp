#include "SpotLightComponent.hpp"

GraphicEngine::GUI::SpotLightComponent::SpotLightComponent() : 
	LightComponent<Engines::Graphic::Shaders::SpotLight>{ "Spot"}
{
	m_positionInput = std::make_shared<InputScalar4>("Position", currentLightParameters.position, 0.05f);
	m_positionInput->onInputScalarEdit([&](glm::vec4 value)
	{
		currentLightParameters.position = value;
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});

	m_directionInput = std::make_shared<InputScalar4>("Direction", currentLightParameters.direction, 0.05f);
	m_directionInput->onInputScalarEdit([&](glm::vec4 value)
	{
		currentLightParameters.direction = value;
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});

	m_innerCutoffInput = std::make_shared<InputScalar>("Inner cutoff", glm::degrees(glm::acos(currentLightParameters.innerCutOff)), 0.05f);
	m_innerCutoffInput->onInputScalarEdit([&](float value)
	{
		currentLightParameters.innerCutOff = glm::cos(glm::radians(value));
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});

	m_outterCutoffInput = std::make_shared<InputScalar>("Outter cutoff", glm::degrees(glm::acos(currentLightParameters.outterCutOff)), 0.05f);
	m_outterCutoffInput->onInputScalarEdit([&](float value)
	{
		currentLightParameters.outterCutOff = glm::cos(glm::radians(value));
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});

	m_constantInput = std::make_shared<InputScalar>("Constant", currentLightParameters.constant, 0.005f);
	m_constantInput->onInputScalarEdit([&](float value)
	{
		currentLightParameters.constant = value;
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});

	m_linearInput = std::make_shared<InputScalar>("Linear", currentLightParameters.linear, 0.005f);
	m_linearInput->onInputScalarEdit([&](float value)
	{
		currentLightParameters.linear = value;
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});

	m_quadrictInput = std::make_shared<InputScalar>("Quadric", currentLightParameters.quadric, 0.005f);
	m_quadrictInput->onInputScalarEdit([&](float value)
	{
		currentLightParameters.quadric = value;
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});
}

void GraphicEngine::GUI::SpotLightComponent::drawEditTools()
{
	m_positionInput->draw();
	m_directionInput->draw();
	m_innerCutoffInput->draw();
	m_outterCutoffInput->draw();
	m_constantInput->draw();
	m_linearInput->draw();
	m_quadrictInput->draw();
}

void GraphicEngine::GUI::SpotLightComponent::updateRestComponents()
{
	m_positionInput->value = currentLightParameters.position;
	m_directionInput->value = currentLightParameters.direction;
	m_innerCutoffInput->value = glm::degrees(glm::acos(currentLightParameters.innerCutOff));
	m_outterCutoffInput->value = glm::degrees(glm::acos(currentLightParameters.outterCutOff));
	m_constantInput->value = currentLightParameters.constant;
	m_linearInput->value = currentLightParameters.linear;
	m_quadrictInput->value = currentLightParameters.quadric;
}
