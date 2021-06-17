#pragma once

#include "../Engines/Graphic/Shaders/Models/WindParameters.hpp"
#include "../Core/Subject.hpp"

namespace GraphicEngine::Services
{
	class WindManager
	{
	public:
		WindManager(std::shared_ptr<Core::Configuration> cfg);

		Engines::Graphic::Shaders::WindParameters getWindParameters();

		void onUpdateWindParameters(std::function<void(Engines::Graphic::Shaders::WindParameters)> callback);

		void setWindParameters(Engines::Graphic::Shaders::WindParameters windParameters);

		void setWindSpeed(float speed);
		void setWindDirection(glm::vec2 direction);

	private:
		Engines::Graphic::Shaders::WindParameters m_windParameters{};
		Core::Subject<Engines::Graphic::Shaders::WindParameters> m_updateWindParametersSubject;
	};
}
