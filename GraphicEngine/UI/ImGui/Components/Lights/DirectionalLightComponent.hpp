#pragma once

#include "LightComponent.hpp"
#include "../../Widgets/InputFloat.hpp"
#include "../../../../Engines/Graphic/Shaders/Models/Light.hpp"

namespace GraphicEngine::GUI
{
	class DirectionalLightComponent : public LightComponent<Engines::Graphic::Shaders::DirectionalLight>
	{
	public:
		DirectionalLightComponent();

	protected:
		// Inherited via LightComponent
		virtual void drawEditTools() override;
		virtual void updateRestComponents() override;

		std::shared_ptr<InputScalar4> m_inputScalar;
	};
}