#include "DirectionalLightComponent.hpp"

GraphicEngine::GUI::DirectionalLightComponent::DirectionalLightComponent():
	LightComponent<Engines::Graphic::Shaders::DirectionalLight>{ "Directional"}
{
	m_inputScalar = std::make_shared<InputScalar4>("Direction", currentLightParameters.direction, 0.05f);
	m_inputScalar->onInputScalarEdit([&](glm::vec4 value)
	{
		currentLightParameters.direction = value;
		currentLightParameters.calculateLigthSpace();
		m_lightEditedSubject.notify(selectedItem, currentLightParameters);
	});
}

void GraphicEngine::GUI::DirectionalLightComponent::drawEditTools()
{
	m_inputScalar->draw();
}

void GraphicEngine::GUI::DirectionalLightComponent::updateRestComponents()
{
	m_inputScalar->value = currentLightParameters.direction;
}
