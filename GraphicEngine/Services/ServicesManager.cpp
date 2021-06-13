#include "ServicesManager.hpp"

GraphicEngine::Services::ServicesManager::ServicesManager(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
	std::shared_ptr<Services::ModelManager> modelManager,
	std::shared_ptr<Services::LightManager> lightManager,
	std::shared_ptr<Services::ViewportManager> viewportManager,
	std::shared_ptr<Services::RenderingOptionsManager> renderingOptionsManager,
	std::shared_ptr<Services::WindManager> windManager)
{
	addService(cameraControllerManager);
	addService(modelManager);
	addService(lightManager);
	addService(viewportManager);
	addService(renderingOptionsManager);
	addService(windManager);
}