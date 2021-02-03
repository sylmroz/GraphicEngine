#include "LightManagerWindow.hpp"
#include "../Widgets/TabBar.hpp"


GraphicEngine::GUI::LightManagerWindow::LightManagerWindow(std::shared_ptr<Services::LightManager> lightManager):
	m_lightManager{ lightManager }
{
	m_container = std::make_shared<CollapsingHeader>("Light settings");
	
	m_directionalLightComponent = std::make_shared<DirectionalLightComponent>();
	m_directionalLightComponent->elementsCount = m_lightManager->getDirectionalLights().size();
	m_directionalLightComponent->onSelectedItem([&](int index)
	{
		m_directionalLightComponent->currentLightParameters = m_lightManager->getDirectionalLight(index);
	});
	m_directionalLightComponent->onAddLightClicked([&]() {
		m_lightManager->addDirectionalLight(Engines::Graphic::Shaders::DirectionalLight());
		m_directionalLightComponent->elementsCount = m_lightManager->getDirectionalLights().size();
		m_directionalLightComponent->setSelectedItem(m_directionalLightComponent->elementsCount - 1);
	});
	m_directionalLightComponent->onDeleteLightClicked([&](int index) {
		m_lightManager->deletetDirectionalLight(index);
		m_directionalLightComponent->elementsCount = m_lightManager->getDirectionalLights().size();
	});
	m_directionalLightComponent->onLightEdited([&](int index, Engines::Graphic::Shaders::DirectionalLight light)
	{
		m_lightManager->settDirectionalLight(light, index);
	});

	auto directionalLightTabBarItem = std::make_shared<TabBarItem>("Directional");
	directionalLightTabBarItem->addChildren(m_directionalLightComponent);
	
	m_pointLightComponent = std::make_shared<PointLightComponent>();
	m_pointLightComponent->elementsCount = m_lightManager->getPointLights().size();
	m_pointLightComponent->onSelectedItem([&](int index)
	{
		m_pointLightComponent->currentLightParameters = m_lightManager->getPointLight(index);
	});
	m_pointLightComponent->onAddLightClicked([&]() {
		m_lightManager->addPointLight(Engines::Graphic::Shaders::PointLight());
		m_pointLightComponent->elementsCount = m_lightManager->getPointLights().size();
		m_pointLightComponent->setSelectedItem(m_pointLightComponent->elementsCount - 1);
	});
	m_pointLightComponent->onDeleteLightClicked([&](int index) {
		m_lightManager->deletePointLight(index);
		m_pointLightComponent->elementsCount = m_lightManager->getPointLights().size();
	});
	m_pointLightComponent->onLightEdited([&](int index, Engines::Graphic::Shaders::PointLight light)
	{
		m_lightManager->setPointLight(light, index);
	});

	auto pointLightTabBarItem = std::make_shared<TabBarItem>("Point");
	pointLightTabBarItem->addChildren(m_pointLightComponent);
	
	m_spotLightComponent = std::make_shared<SpotLightComponent>();
	m_spotLightComponent->elementsCount = m_lightManager->getSpotLights().size();
	m_spotLightComponent->onSelectedItem([&](int index)
	{
		m_spotLightComponent->currentLightParameters = m_lightManager->getSpotLight(index);
	});
	m_spotLightComponent->onAddLightClicked([&]() {
		m_lightManager->addSpotLight(Engines::Graphic::Shaders::SpotLight());
		m_spotLightComponent->elementsCount = m_lightManager->getSpotLights().size();
		m_pointLightComponent->setSelectedItem(m_spotLightComponent->elementsCount - 1);
	});
	m_spotLightComponent->onDeleteLightClicked([&](int index) {
		m_lightManager->deleteSpotLight(index);
		m_spotLightComponent->elementsCount = m_lightManager->getSpotLights().size();
	});
	m_spotLightComponent->onLightEdited([&](int index, Engines::Graphic::Shaders::SpotLight light)
	{
		m_lightManager->setSpotLight(light, index);
	});

	auto spotLightTabBarItem = std::make_shared<TabBarItem>("Spot");
	spotLightTabBarItem->addChildren(m_spotLightComponent);

	auto lightTabBar = std::make_shared<TabBar>("##lights");
	lightTabBar->addChildren(directionalLightTabBarItem);
	lightTabBar->addChildren(pointLightTabBarItem);
	lightTabBar->addChildren(spotLightTabBarItem);

	m_container->addChildren(lightTabBar);
}

void GraphicEngine::GUI::LightManagerWindow::draw()
{
	m_container->draw();
}
