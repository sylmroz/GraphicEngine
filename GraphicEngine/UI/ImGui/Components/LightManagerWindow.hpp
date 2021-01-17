#pragma once

#include "../Widgets/CollapsingHeader.hpp"
#include "../../../Services/LightManager.hpp"

#include "Lights/DirectionalLightComponent.hpp"
#include "Lights/PointLightCompoment.hpp"
#include "Lights/SpotLightComponent.hpp"

namespace GraphicEngine::GUI
{
	class LightManagerWindow : public Widget
	{
	public:
		LightManagerWindow(std::shared_ptr<Services::LightManager> lightManager);
	protected:
		// Inherited via Widget
		virtual void draw() override;

		std::shared_ptr<Services::LightManager> m_lightManager;
		std::shared_ptr<CollapsingHeader> m_container;

		std::shared_ptr<DirectionalLightComponent> m_directionalLightComponent;
		std::shared_ptr<PointLightComponent> m_pointLightComponent;
		std::shared_ptr<SpotLightComponent> m_spotLightComponent;
	};
}