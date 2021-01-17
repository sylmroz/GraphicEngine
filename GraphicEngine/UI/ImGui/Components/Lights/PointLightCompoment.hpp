#pragma once

#include "LightComponent.hpp"
#include "../../Widgets/InputFloat.hpp"
#include "../../../../Engines/Graphic/Shaders/Models/Light.hpp"

namespace GraphicEngine::GUI
{
	class PointLightComponent : public LightComponent<Engines::Graphic::Shaders::PointLight>
	{
	public:
		PointLightComponent();

	protected:
		// Inherited via LightComponent
		virtual void drawEditTools() override;
		virtual void updateRestComponents() override;

		std::shared_ptr<InputScalar4> m_positionInput;
		std::shared_ptr<InputScalar> m_constantInput;
		std::shared_ptr<InputScalar> m_linearInput;
		std::shared_ptr<InputScalar> m_quadrictInput;
	};
}