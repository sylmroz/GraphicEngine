#include "LightComponent.hpp"

GraphicEngine::GUI::LightColorComponent::LightColorComponent(GraphicEngine::Engines::Graphic::Shaders::LightColor lightColor) :
	lightColor{ lightColor }
{
	m_diffuseColorEdit = std::make_shared<ColorEdit>("Diffuse", lightColor.diffuse);
	m_diffuseColorEdit->subscribe([&](glm::vec4 color)
	{
		this->lightColor.diffuse = color;
		m_lightColorSubject.notify(this->lightColor);
	});

	m_specularColorEdit = std::make_shared<ColorEdit>("Specular", lightColor.specular);
	m_specularColorEdit->subscribe([&](glm::vec4 color)
	{
		this->lightColor.specular = color;
		m_lightColorSubject.notify(this->lightColor);
	});

	m_ambientColorEdit = std::make_shared<ColorEdit>("Ambient", lightColor.ambient);
	m_ambientColorEdit->subscribe([&](glm::vec4 color)
	{
		this->lightColor.ambient = color;
		m_lightColorSubject.notify(this->lightColor);
	});

	addChildren(m_diffuseColorEdit);
	addChildren(m_specularColorEdit);
	addChildren(m_ambientColorEdit);
}

void GraphicEngine::GUI::LightColorComponent::onLightColor(std::function<void(Engines::Graphic::Shaders::LightColor)> callback)
{
	m_lightColorSubject.subscribe(callback);
}

void GraphicEngine::GUI::LightColorComponent::setCurrentColors(Engines::Graphic::Shaders::LightColor lightColor)
{
	this->lightColor = lightColor;
	m_diffuseColorEdit->color = lightColor.diffuse;
	m_specularColorEdit->color = lightColor.specular;
	m_ambientColorEdit->color = lightColor.ambient;
}

void GraphicEngine::GUI::LightColorComponent::draw()
{
	for (auto& children : m_childrens)
	{
		children->draw();
	}
}
