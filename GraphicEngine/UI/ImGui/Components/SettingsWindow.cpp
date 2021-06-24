#include "SettingsWindow.hpp"

GraphicEngine::GUI::SettingWindow::SettingWindow(std::shared_ptr<ViewportSettingWindow> settingWindow, std::shared_ptr<RenderingSettingsWindow> renderingSettingWindow, std::shared_ptr<CameraManagerWindow> cameraManagerWindow, std::shared_ptr<LightManagerWindow> lightManager,
	std::shared_ptr<WindManagerWindow> windManagerWindow)
{
	m_body = std::make_shared<WindowBody>("Settings");
	m_body->addChildren(settingWindow);
	m_body->addChildren(renderingSettingWindow);
	m_body->addChildren(cameraManagerWindow);
	m_body->addChildren(lightManager);
	m_body->addChildren(windManagerWindow);
}

void GraphicEngine::GUI::SettingWindow::init()
{
	m_body->init();
}

void GraphicEngine::GUI::SettingWindow::draw()
{
	m_body->draw();
}
