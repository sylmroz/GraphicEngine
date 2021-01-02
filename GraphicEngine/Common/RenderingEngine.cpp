#include "RenderingEngine.hpp"

GraphicEngine::RenderingEngine::RenderingEngine(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager, std::shared_ptr<Services::ModelManager> modelManager, std::shared_ptr<Core::EventManager> eventManager, std::shared_ptr<Common::UI> ui, std::shared_ptr<Core::Configuration> cfg) :
	m_cameraControllerManager{ cameraControllerManager },
	m_modelManager{ modelManager },
	m_eventManager{ eventManager },
	m_ui{ ui },
	m_cfg{ cfg }
{
	/*auto cylinder = Engines::Graphic::ConeGenerator<Common::VertexPNW>{}.getModel(glm::vec3(0.0f), 0.5f, 2.0f, glm::ivec3(20, 2, 2), false, true, Engines::Graphic::TriangleDirection::CounterClockwise);
	m_modelManager->addModel(cylinder);*/
	// m_models.push_back(cylinder);
	/*m_models.front()->setScale(m_cfg->getProperty<float>("scene:object:scale"));
	m_models.front()->setRotate(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:rotate")));
	m_models.front()->setPosition(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:position")));

	m_models.front()->getMeshes().front()->setRotate(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:rotate")));*/
	/*auto cube = Engines::Graphic::CuboidGenerator<Common::VertexPN>{}.getModel(glm::vec3(-0.25f), glm::vec3(0.25), glm::ivec3(3), Engines::Graphic::GeneratingPosition::Corner, Engines::Graphic::TriangleDirection::CounterClockwise);
	m_modelManager->addModel(cube);*/

	auto models = Modules::AssimpModelImporter<Common::VertexPN>{}.read(m_cfg->getProperty<std::string>("scene:object:path"));
	models.front()->setScale(m_cfg->getProperty<float>("scene:object:scale"));
	models.front()->setRotate(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:rotate")));
	models.front()->setPosition(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:position")));

	for (auto model : models)
		m_modelManager->addModel(model);

	auto sphere = Engines::Graphic::SphereGenerator<Common::VertexPN>{}.getModel(glm::vec3(0.0f), 0.25, glm::ivec2(20), Engines::Graphic::TriangleDirection::CounterClockwise);
	m_modelManager->addModel(sphere);
	auto backgroundColorEdit = std::make_shared<GUI::ColorEdit>(backgroudColor);
	backgroundColorEdit->subscribe([&](glm::vec4 color)
	{
		backgroudColor = color;
	});
	m_ui->addWidget(backgroundColorEdit);

	auto displayWireframeCheckbox = std::make_shared<GUI::Checkbox>("wireframe", displayWireframe);
	displayWireframeCheckbox->subscribe([&](bool checked)
	{
		displayWireframe = checked;
	});
	m_ui->addWidget(displayWireframeCheckbox);

	auto displaySolidCheckbox = std::make_shared<GUI::Checkbox>("solid", displaySolid);
	displaySolidCheckbox->subscribe([&](bool checked)
	{
		displaySolid = checked;
	});
	m_ui->addWidget(displaySolidCheckbox);

	auto displayNormalCheckbox = std::make_shared<GUI::Checkbox>("normal", displayNormal);
	displayNormalCheckbox->subscribe([&](bool checked)
	{
		displayNormal = checked;
	});
	m_ui->addWidget(displayNormalCheckbox);
}
