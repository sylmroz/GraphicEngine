#include "RenderingEngine.hpp"

GraphicEngine::RenderingEngine::RenderingEngine(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
	std::shared_ptr<Services::ModelManager> modelManager,
	std::shared_ptr<Services::LightManager> lightManager,
	std::shared_ptr<Services::ViewportManager> viewportManager,
	std::shared_ptr<Services::RenderingOptionsManager> renderingOptionsManager,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Core::Timer> timer,
	std::shared_ptr<Common::UI> ui,
	std::shared_ptr<Core::Configuration> cfg) :
	m_cameraControllerManager{ cameraControllerManager },
	m_lightManager{ lightManager },
	m_modelManager{ modelManager },
	m_viewportManager{ viewportManager },
	m_renderingOptionsManager{ renderingOptionsManager },
	m_eventManager{ eventManager },
	m_timer{ timer },
	m_ui{ ui },
	m_cfg{ cfg }
{}
