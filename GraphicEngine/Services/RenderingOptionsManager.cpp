#include "RenderingOptionsManager.hpp"

GraphicEngine::Services::RenderingOptionsManager::RenderingOptionsManager(std::shared_ptr<Core::Configuration> cfg)
{
	try {
		renderingOptions.shadowRendering.directional = cfg->getProperty<bool>("rendering options:shadows:directional");
		renderingOptions.shadowRendering.point = cfg->getProperty<bool>("rendering options:shadows:point");
		renderingOptions.shadowRendering.spot = cfg->getProperty<bool>("rendering options:shadows:spot");
		renderingOptions.ambientOcclusion = cfg->getProperty<bool>("rendering options:ambinent occlusion");
		renderingOptions.globalIllumination = cfg->getProperty<bool>("rendering options:global illuminiation");
	}

	catch (std::runtime_error err)
	{

	}

	catch (...)
	{

	}
}

void GraphicEngine::Services::RenderingOptionsManager::onUpdateOptions(std::function<void(Engines::Graphic::Shaders::RenderingOptions)> callback)
{
	m_updateOptionsSubject.subscribe(callback);
}

void GraphicEngine::Services::RenderingOptionsManager::setRenderingOptions(Engines::Graphic::Shaders::RenderingOptions renderingOptions)
{
	this->renderingOptions = renderingOptions;
	m_updateOptionsSubject.notify(this->renderingOptions);
}
