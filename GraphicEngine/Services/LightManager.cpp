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

GraphicEngine::Engines::Graphic::Shaders::DirectionalLight GraphicEngine::Services::LightManager::getDirectionalLight(uint32_t index)
{
	if (index < m_directionalLights.size())
		return m_directionalLights[index];
	throw std::out_of_range("Light index out of bound");
}

GraphicEngine::Engines::Graphic::Shaders::PointLight GraphicEngine::Services::LightManager::getPointLight(uint32_t index)
{
	if (index < m_pointLights.size())
		return m_pointLights[index];
	throw std::out_of_range("Light index out of bound");
}

GraphicEngine::Engines::Graphic::Shaders::SpotLight GraphicEngine::Services::LightManager::getSpotLight(uint32_t index)
{
	if (index < m_spotlLights.size())
		return m_spotlLights[index];
	throw std::out_of_range("Light index out of bound");
}

void GraphicEngine::Services::LightManager::settDirectionalLight(Engines::Graphic::Shaders::DirectionalLight light, uint32_t index)
{
	if (index < m_directionalLights.size())
	{
		m_directionalLights[index] = light;
		m_updateDirectionalLightSubject.notify(index, light);
	}
	else 
		throw std::out_of_range("Light index out of bound");
}

void GraphicEngine::Services::LightManager::setPointLight(Engines::Graphic::Shaders::PointLight light, uint32_t index)
{
	if (index < m_pointLights.size())
	{
		m_pointLights[index] = light;
		m_updatePointLightSubject.notify(index, light);
	}
	else
		throw std::out_of_range("Light index out of bound");
}

void GraphicEngine::Services::LightManager::setSpotLight(Engines::Graphic::Shaders::SpotLight light, uint32_t index)
{
	if (index < m_spotlLights.size())
	{
		m_spotlLights[index] = light;
		m_updateSpotLightSubject.notify(index, light);
	}
	else
		throw std::out_of_range("Light index out of bound");
}

void GraphicEngine::Services::LightManager::addDirectionalLight(Engines::Graphic::Shaders::DirectionalLight light)
{
	m_directionalLights.push_back(light);
	m_updateDirectionalLightsSubject.notify(m_directionalLights);
}

void GraphicEngine::Services::LightManager::addPointLight(Engines::Graphic::Shaders::PointLight light)
{
	m_pointLights.push_back(light);
	m_updatePointLightsSubject.notify(m_pointLights);
}

void GraphicEngine::Services::LightManager::addSpotLight(Engines::Graphic::Shaders::SpotLight light)
{
	m_spotlLights.push_back(light);
	m_updateSpotLightsSubject.notify(m_spotlLights);
}

void GraphicEngine::Services::LightManager::deletetDirectionalLight(uint32_t index)
{
	if (index < m_directionalLights.size())
	{
		m_directionalLights.erase(m_directionalLights.begin() + index);
		m_updateDirectionalLightsSubject.notify(m_directionalLights);
	}
	else 
		throw std::out_of_range("Light index out of bound");
}

void GraphicEngine::Services::LightManager::deletePointLight(uint32_t index)
{
	if (index < m_pointLights.size())
	{
		m_pointLights.erase(m_pointLights.begin() + index);
		m_updatePointLightsSubject.notify(m_pointLights);
	}
	else
		throw std::out_of_range("Light index out of bound");
}

void GraphicEngine::Services::LightManager::deleteSpotLight(uint32_t index)
{
	if (index < m_spotlLights.size())
	{
		m_spotlLights.erase(m_spotlLights.begin() + index);
		m_updateSpotLightsSubject.notify(m_spotlLights);
	}
	else
		throw std::out_of_range("Light index out of bound");
}

void GraphicEngine::Services::LightManager::onUpdateDirectiionalLights(std::function<void(std::vector<Engines::Graphic::Shaders::DirectionalLight>)> callback)
{
	m_updateDirectionalLightsSubject.subscribe(callback);
}

void GraphicEngine::Services::LightManager::onUpdatePointLights(std::function<void(std::vector<Engines::Graphic::Shaders::PointLight>)> callback)
{
	m_updatePointLightsSubject.subscribe(callback);
}

void GraphicEngine::Services::LightManager::onUpdateSpotlLights(std::function<void(std::vector<Engines::Graphic::Shaders::SpotLight>)> callback)
{
	m_updateSpotLightsSubject.subscribe(callback);
}

void GraphicEngine::Services::LightManager::onUpdateDirectiionalLight(std::function<void(uint32_t, Engines::Graphic::Shaders::DirectionalLight)> callback)
{
	m_updateDirectionalLightSubject.subscribe(callback);
}

void GraphicEngine::Services::LightManager::onUpdatePointLight(std::function<void(uint32_t, Engines::Graphic::Shaders::PointLight)> callback)
{
	m_updatePointLightSubject.subscribe(callback);
}

void GraphicEngine::Services::LightManager::onUpdateSpotlLight(std::function<void(uint32_t, Engines::Graphic::Shaders::SpotLight)> callback)
{
	m_updateSpotLightSubject.subscribe(callback);
}
