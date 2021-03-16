#include "RenderingEngine.hpp"

#include "../Engines/Graphic/3D/ObjectGenerator.hpp"

GraphicEngine::RenderingEngine::RenderingEngine(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
	std::shared_ptr<Services::ModelManager> modelManager,
	std::shared_ptr<Services::LightManager> lightManager,
	std::shared_ptr<Services::ViewportManager> viewportManager,
	std::shared_ptr<Services::RenderingOptionsManager> renderingOptionsManager,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Common::UI> ui,
	std::shared_ptr<Core::Configuration> cfg) :
	m_cameraControllerManager{ cameraControllerManager },
	m_lightManager{ lightManager },
	m_modelManager{ modelManager },
	m_viewportManager{ viewportManager },
	m_renderingOptionsManager{ renderingOptionsManager },
	m_eventManager{ eventManager },
	m_ui{ ui },
	m_cfg{ cfg }
{
	/*auto models = Modules::AssimpModelImporter<Common::VertexPN>{}.read(m_cfg->getProperty<std::string>("scene:object:path"));
	models.front()->setScale(m_cfg->getProperty<float>("scene:object:scale"));
	models.front()->setRotate(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:rotate")));
	models.front()->setPosition(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:position")));

	for (auto model : models)
		m_modelManager->addModel(model);

	auto sphere = Engines::Graphic::SphereGenerator<Common::VertexPN>{}.getModel(glm::vec3(0.0f), 0.25, glm::ivec2(20), Engines::Graphic::TriangleDirection::CounterClockwise);
	m_modelManager->addModel(sphere);

	auto cube = Engines::Graphic::CuboidGenerator<Common::VertexPN>{}.getModel(glm::vec3(2.0f, 0.0f, -2.25f), glm::vec3(2.25f, 0.25f, -2.00f), glm::ivec3(3), Engines::Graphic::GeneratingPosition::Corner, Engines::Graphic::TriangleDirection::CounterClockwise);
	m_modelManager->addModel(cube);

	auto cone = Engines::Graphic::ConeGenerator<Common::VertexPN>{}.getModel(glm::vec3(1.0f, 0.0f, 0.0f), 0.25f, 1.0f, glm::ivec3(20, 2, 2), true, true, Engines::Graphic::TriangleDirection::CounterClockwise);
	m_modelManager->addModel(cone);

	auto cylinder = Engines::Graphic::CylinderGenerator<Common::VertexPN>{}.getModel(glm::vec3(1.0f, 0.0f, 1.0f), 0.25f, 0.125f, 0.5f, glm::ivec3(20, 4, 4), true, true, true, Engines::Graphic::TriangleDirection::CounterClockwise);
	m_modelManager->addModel(cylinder);

	auto plane = Engines::Graphic::PlaneGenerator<Common::VertexPN>{}.getModel(glm::vec2(-300), glm::vec2(300), glm::ivec2(8), Engines::Graphic::GeneratingPosition::Corner, Engines::Graphic::TriangleDirection::CounterClockwise);
	m_modelManager->addModel(plane);*/
}
