#include "LightManager.hpp"

GraphicEngine::Services::LightManager::LightManager(std::shared_ptr<Core::Configuration> cfg)
{
	auto directionLights = cfg->getProperty<std::vector<json>>("lights:directional");
	for (auto directionLight : directionLights)
	{
		m_directionalLights.push_back(Engines::Graphic::Shaders::DirectionalLight(std::make_shared<Core::Configuration>(directionLight)));
	}

	auto pointLights = cfg->getProperty<std::vector<json>>("lights:point");
	for (auto pointLight : pointLights)
	{
		m_pointLights.push_back(Engines::Graphic::Shaders::PointLight(std::make_shared<Core::Configuration>(pointLight)));
	}

	auto spotLights = cfg->getProperty<std::vector<json>>("lights:spot");
	for (auto spotLight : spotLights)
	{
		m_spotlLights.push_back(Engines::Graphic::Shaders::SpotLight(std::make_shared<Core::Configuration>(spotLight)));
	}
}

std::vector<GraphicEngine::Engines::Graphic::Shaders::DirectionalLight> GraphicEngine::Services::LightManager::getDirectionalLights()
{
	return m_directionalLights;
}

std::vector<GraphicEngine::Engines::Graphic::Shaders::PointLight> GraphicEngine::Services::LightManager::getPointLights()
{
	return m_pointLights;
}

std::vector<GraphicEngine::Engines::Graphic::Shaders::SpotLight> GraphicEngine::Services::LightManager::getSpotLights()
{
	return m_spotlLights;
}
