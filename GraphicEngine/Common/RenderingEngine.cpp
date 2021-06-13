#include "RenderingEngine.hpp"

GraphicEngine::RenderingEngine::RenderingEngine(std::shared_ptr<Services::ServicesManager> servicesManager,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Core::Timer> timer,
	std::shared_ptr<Common::UI> ui,
	std::shared_ptr<Core::Configuration> cfg) :
	m_servicesManager{ servicesManager },
	m_cameraControllerManager{ servicesManager->getService<Services::CameraControllerManager>() },
	m_lightManager{ servicesManager->getService<Services::LightManager>() },
	m_modelManager{ servicesManager->getService<Services::ModelManager>() },
	m_viewportManager{ servicesManager->getService<Services::ViewportManager>() },
	m_renderingOptionsManager{ servicesManager->getService<Services::RenderingOptionsManager>() },
	m_windManager{ servicesManager->getService<Services::WindManager>() },
	m_eventManager{ eventManager },
	m_timer{ timer },
	m_ui{ ui },
	m_cfg{ cfg }
{}
