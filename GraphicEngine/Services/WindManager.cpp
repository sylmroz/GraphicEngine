#include "WindManager.hpp"
#include <filesystem>

GraphicEngine::Services::WindManager::WindManager(std::shared_ptr<Core::Configuration> cfg)
{
	auto windParametersJson = cfg->getProperty<json>("scene:wind:parameters");
	m_windParameters = Engines::Graphic::Shaders::WindParameters(std::make_shared<Core::Configuration>(windParametersJson));
	m_windTexturePath = cfg->getProperty<std::string>("scene:wind:generator:path");
	loadWind();
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

void GraphicEngine::Services::WindManager::setWindDirection(glm::vec2 direction)
{
	m_windParameters.direction = direction;
	m_updateWindParametersSubject.notify(m_windParameters);
}

void GraphicEngine::Services::WindManager::generateWindTexture(uint32_t resolution, float speed)
{
	m_windTexture = Engines::Graphic::WindGenerator::generate(resolution, speed);
	m_resolution = resolution;
	m_textureObject.reset();
	cv::imwrite(m_windTexturePath, m_windTexture);
}

uint32_t GraphicEngine::Services::WindManager::getWindTextureResolution()
{
	return m_resolution;
}

void GraphicEngine::Services::WindManager::loadWind()
{
	if (std::filesystem::exists(m_windTexturePath))
	{
		m_windTexture = cv::imread(m_windTexturePath, cv::IMREAD_UNCHANGED);
		m_resolution = m_windTexture.rows;
	}
	else
	{
		generateWindTexture(1024, 1.0);
	}
}
