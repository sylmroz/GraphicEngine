#include "RenderingSettingsWindow.hpp"

GraphicEngine::GUI::RenderingSettingsWindow::RenderingSettingsWindow(std::shared_ptr<Services::RenderingOptionsManager> renderingOptionsManager)
{
	m_renderingOptionsManager = renderingOptionsManager;
	m_container = std::make_shared<CollapsingHeader>("Rendering settings");

	auto directionalShadows = std::make_shared<GUI::Checkbox>("directional shadows", m_renderingOptionsManager->renderingOptions.shadowRendering.directional);
	directionalShadows->subscribe([&](bool checked)
		{
			m_renderingOptionsManager->renderingOptions.shadowRendering.directional = checked;
			m_renderingOptionsManager->setRenderingOptions(m_renderingOptionsManager->renderingOptions);
		});
	m_container->addChildren(directionalShadows);

	auto pointShadows = std::make_shared<GUI::Checkbox>("point shadows", m_renderingOptionsManager->renderingOptions.shadowRendering.point);
	pointShadows->subscribe([&](bool checked)
		{
			m_renderingOptionsManager->renderingOptions.shadowRendering.point = checked;
			m_renderingOptionsManager->setRenderingOptions(m_renderingOptionsManager->renderingOptions);
		});
	m_container->addChildren(pointShadows);

	auto spotShadows = std::make_shared<GUI::Checkbox>("spot shadows", m_renderingOptionsManager->renderingOptions.shadowRendering.spot);
	spotShadows->subscribe([&](bool checked)
		{
			m_renderingOptionsManager->renderingOptions.shadowRendering.spot = checked;
			m_renderingOptionsManager->setRenderingOptions(m_renderingOptionsManager->renderingOptions);
		});
	m_container->addChildren(spotShadows);

	auto globalIllumination = std::make_shared<GUI::Checkbox>("global illumination", m_renderingOptionsManager->renderingOptions.globalIllumination);
	globalIllumination->subscribe([&](bool checked)
		{
			m_renderingOptionsManager->renderingOptions.globalIllumination = checked;
			m_renderingOptionsManager->setRenderingOptions(m_renderingOptionsManager->renderingOptions);
		});
	m_container->addChildren(globalIllumination);

	auto ambientOcclusion = std::make_shared<GUI::Checkbox>("ambient occlusion", m_renderingOptionsManager->renderingOptions.ambientOcclusion);
	ambientOcclusion->subscribe([&](bool checked)
		{
			m_renderingOptionsManager->renderingOptions.ambientOcclusion = checked;
			m_renderingOptionsManager->setRenderingOptions(m_renderingOptionsManager->renderingOptions);
		});
	m_container->addChildren(ambientOcclusion);
}

void GraphicEngine::GUI::RenderingSettingsWindow::draw()
{
	m_container->draw();
}
