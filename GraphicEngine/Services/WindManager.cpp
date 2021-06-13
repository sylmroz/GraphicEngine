#include "WindManager.hpp"

GraphicEngine::Services::WindManager::WindManager(std::shared_ptr<Core::Configuration> cfg)
{
	auto windParametersJson = cfg->getProperty<json>("scene:wind:parameters");
	m_windParameters = Engines::Graphic::Shaders::WindParameters(std::make_shared<Core::Configuration>(windParametersJson));
}

GraphicEngine::Engines::Graphic::Shaders::WindParameters GraphicEngine::Services::WindManager::getWindParameters()
{
	return m_windParameters;
}

void GraphicEngine::Services::WindManager::onUpdateWindParameters(std::function<void(Engines::Graphic::Shaders::WindParameters)> callback)
{
	m_updateWindParametersSubject.subscribe(callback);
}

void GraphicEngine::Services::WindManager::setWindParameters(Engines::Graphic::Shaders::WindParameters windParameters)
{
	m_windParameters = windParameters;
	m_updateWindParametersSubject.notify(m_windParameters);
}

void GraphicEngine::Services::WindManager::setWindSpeed(float speed)
{
	m_windParameters.speed = speed;
	m_updateWindParametersSubject.notify(m_windParameters);
}
