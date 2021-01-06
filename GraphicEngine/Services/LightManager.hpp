#pragma once

#include "../Engines/Graphic/Shaders/Models/Light.hpp"
#include <vector>
#include <memory>
#include "../Core/Configuration.hpp"

namespace GraphicEngine::Services
{
	class LightManager
	{
	public:
		LightManager() = default;
		LightManager(std::shared_ptr<Core::Configuration> cfg);

		std::vector<Engines::Graphic::Shaders::DirectionalLight> getDirectionalLights();
		std::vector<Engines::Graphic::Shaders::PointLight> getPointLights();
		std::vector<Engines::Graphic::Shaders::SpotLight> getSpotLights();

	private:
		std::vector<Engines::Graphic::Shaders::DirectionalLight> m_directionalLights;
		std::vector<Engines::Graphic::Shaders::PointLight> m_pointLights;
		std::vector<Engines::Graphic::Shaders::SpotLight> m_spotlLights;
	};
}