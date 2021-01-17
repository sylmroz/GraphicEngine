#pragma once

#include "LightComponent.hpp"
#include "../../Widgets/InputFloat.hpp"
#include "../../../../Engines/Graphic/Shaders/Models/Light.hpp"

namespace GraphicEngine::GUI
{
	class SpotLightComponent : public LightComponent<Engines::Graphic::Shaders::SpotLight>
	{
	public:
		SpotLightComponent();

	protected:
		// Inherited via LightComponent
		virtual void drawEditTools() override;
		virtual void updateRestComponents() override;

		std::shared_ptr<InputScalar4> m_positionInput;
		std::shared_ptr<InputScalar4> m_directionInput;

		std::shared_ptr<InputScalar> m_innerCutoffInput;
		std::shared_ptr<InputScalar> m_outterCutoffInput;

		std::shared_ptr<InputScalar> m_constantInput;
		std::shared_ptr<InputScalar> m_linearInput;
		std::shared_ptr<InputScalar> m_quadrictInput;
	};
}