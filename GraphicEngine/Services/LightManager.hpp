#pragma once

#include "../Engines/Graphic/Shaders/Models/Light.hpp"
#include <memory>
#include "../Core/Configuration.hpp"
#include "../Core/Subject.hpp"

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

		Engines::Graphic::Shaders::DirectionalLight getDirectionalLight(uint32_t index);
		Engines::Graphic::Shaders::PointLight getPointLight(uint32_t index);
		Engines::Graphic::Shaders::SpotLight getSpotLight(uint32_t index);

		void settDirectionalLight(Engines::Graphic::Shaders::DirectionalLight light, uint32_t index);
		void setPointLight(Engines::Graphic::Shaders::PointLight light, uint32_t index);
		void setSpotLight(Engines::Graphic::Shaders::SpotLight light, uint32_t index);

		void addDirectionalLight(Engines::Graphic::Shaders::DirectionalLight light);
		void addPointLight(Engines::Graphic::Shaders::PointLight light);
		void addSpotLight(Engines::Graphic::Shaders::SpotLight light);

		void deletetDirectionalLight(uint32_t index);
		void deletePointLight(uint32_t index);
		void deleteSpotLight(uint32_t index);

		void onUpdateDirectiionalLights(std::function<void(std::vector<Engines::Graphic::Shaders::DirectionalLight>)> callback);
		void onUpdatePointLights(std::function<void(std::vector<Engines::Graphic::Shaders::PointLight>)> callback);
		void onUpdateSpotlLights(std::function<void(std::vector<Engines::Graphic::Shaders::SpotLight>)> callback);

		void onUpdateDirectiionalLight(std::function<void(uint32_t, Engines::Graphic::Shaders::DirectionalLight)> callback);
		void onUpdatePointLight(std::function<void(uint32_t, Engines::Graphic::Shaders::PointLight)> callback);
		void onUpdateSpotlLight(std::function<void(uint32_t, Engines::Graphic::Shaders::SpotLight)> callback);

	private:
		std::vector<Engines::Graphic::Shaders::DirectionalLight> m_directionalLights;
		std::vector<Engines::Graphic::Shaders::PointLight> m_pointLights;
		std::vector<Engines::Graphic::Shaders::SpotLight> m_spotlLights;

		Core::Subject<std::vector<Engines::Graphic::Shaders::DirectionalLight>> m_updateDirectionalLightsSubject;
		Core::Subject<std::vector<Engines::Graphic::Shaders::PointLight>> m_updatePointLightsSubject;
		Core::Subject<std::vector<Engines::Graphic::Shaders::SpotLight>> m_updateSpotLightsSubject;

		Core::Subject<uint32_t, Engines::Graphic::Shaders::DirectionalLight> m_updateDirectionalLightSubject;
		Core::Subject<uint32_t, Engines::Graphic::Shaders::PointLight> m_updatePointLightSubject;
		Core::Subject<uint32_t, Engines::Graphic::Shaders::SpotLight> m_updateSpotLightSubject;
	};
}