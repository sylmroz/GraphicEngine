#include "ViewportSettingsWindow.hpp"

GraphicEngine::GUI::ViewportSettingWindow::ViewportSettingWindow(std::shared_ptr<Services::ViewportManager> viewportManager):
	m_viewportManager{ viewportManager }
{
	m_container = std::make_shared<CollapsingHeader>("Viewport settings");

	auto backgroundColorEdit = std::make_shared<GUI::ColorEdit>(m_viewportManager->backgroudColor);
	backgroundColorEdit->subscribe([&](glm::vec4 color)
	{
		m_viewportManager->backgroudColor = color;
	});
	m_container->addChildren(backgroundColorEdit);

	auto displayWireframeCheckbox = std::make_shared<GUI::Checkbox>("wireframe", m_viewportManager->displayWireframe);
	displayWireframeCheckbox->subscribe([&](bool checked)
	{
		m_viewportManager->displayWireframe = checked;
	});
	m_container->addChildren(displayWireframeCheckbox);

	auto displaySolidCheckbox = std::make_shared<GUI::Checkbox>("solid", m_viewportManager->displaySolid);
	displaySolidCheckbox->subscribe([&](bool checked)
	{
		m_viewportManager->displaySolid = checked;
	});
	m_container->addChildren(displaySolidCheckbox);

	auto displayNormalCheckbox = std::make_shared<GUI::Checkbox>("normal", m_viewportManager->displayNormal);
	displayNormalCheckbox->subscribe([&](bool checked)
	{
		m_viewportManager->displayNormal = checked;
	});
	m_container->addChildren(displayNormalCheckbox);

	auto displaySkyBox = std::make_shared<GUI::Checkbox>("skybox", m_viewportManager->displaySkybox);
	displaySkyBox->subscribe([&](bool checked)
	{
		m_viewportManager->displaySkybox = checked;
	});
	m_container->addChildren(displaySkyBox);
}

void GraphicEngine::GUI::ViewportSettingWindow::draw()
{
	m_container->draw();
}
