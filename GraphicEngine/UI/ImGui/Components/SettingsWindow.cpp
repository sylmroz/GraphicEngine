#include "SettingsWindow.hpp"

GraphicEngine::GUI::SettingWindow::SettingWindow(std::shared_ptr<ViewportSettingWindow> settingWindow)
{
	m_body = std::make_shared<WindowBody>("Settings");
	m_body->addChildren(settingWindow);
}

void GraphicEngine::GUI::SettingWindow::draw()
{
	m_body->draw();
}
