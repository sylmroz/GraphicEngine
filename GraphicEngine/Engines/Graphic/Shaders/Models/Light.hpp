#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../../../../Core/Utils/ObjectConverter.hpp"
#include "../../../../Core/Configuration.hpp"
#include <iostream>

namespace GraphicEngine::Engines::Graphic::Shaders
{

	struct LightColor
	{
		LightColor() = default;
		LightColor(std::shared_ptr<Core::Configuration> cfg)
		{
			diffuse = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("diffuse")), 1.0f);
			ambient = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("ambient")), 1.0f);
			specular = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("specular")), 1.0f);
		}

		LightColor(glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 specular) :
			diffuse{ diffuse }, ambient{ ambient }, specular{ specular } {}
		
		alignas(16) glm::vec4 diffuse{ 0.5f, 0.5f, 0.5f, 1.0f };
		alignas(16) glm::vec4 ambient{ 0.1f, 0.1f, 0.1f, 1.0f };
		alignas(16) glm::vec4 specular{ 0.5f, 0.5f, 0.5f, 1.0f };
	};

	struct DirectionalLight
	{
		DirectionalLight() = default;
		DirectionalLight(std::shared_ptr<Core::Configuration> cfg)
		{
			direction = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("direction")), 1.0f);
			// Need to be done in this way - normally throw error
			auto props = cfg->getProperty<json>("color");
			props.items();
			color = LightColor(std::make_shared<Core::Configuration>(props));
		}
		DirectionalLight(glm::vec4 direction, LightColor color) :
			direction{ direction }, color{ color } {}
		
		alignas(16) glm::vec4 direction{ 1.0f, -1.0f, 1.0f, 1.0f };
		alignas(16) LightColor color;
	};

	struct PointLight
	{
		PointLight() = default;
		PointLight(std::shared_ptr<Core::Configuration> cfg)
		{
			position = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("position")), 1.0f);
			constant = cfg->getProperty<float>("constant");
			linear = cfg->getProperty<float>("linear");
			quadric = cfg->getProperty<float>("quadric");
			auto props = cfg->getProperty<json>("color");
			props.items();
			color = LightColor(std::make_shared<Core::Configuration>(props));
		}
		PointLight(glm::vec4 position, float constant, float linear, float quadric, LightColor color) :
			position{ position }, constant{ constant }, linear{ linear }, quadric{ quadric }, color{ color } {}

		alignas(16) glm::vec4 position{ 2.0f, 2.0f, 2.0f, 1.0f };
		float constant{ 1.0f };
		float linear{ 0.22 };
		float quadric{ 0.2 };
		LightColor color;
	};

	struct SpotLight
	{
		SpotLight() = default;
		SpotLight(std::shared_ptr<Core::Configuration> cfg)
		{
			position = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("position")), 1.0f);
			direction = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("direction")), 1.0f);
			innerCutOff = glm::cos(glm::radians(cfg->getProperty<float>("inner cutoff")));
			outterCutOff = glm::cos(glm::radians(cfg->getProperty<float>("outter cutoff")));
			constant = cfg->getProperty<float>("constant");
			linear = cfg->getProperty<float>("linear");
			quadric = cfg->getProperty<float>("quadric");
			auto props = cfg->getProperty<json>("color");
			props.items();
			color = LightColor(std::make_shared<Core::Configuration>(props));
		}
		SpotLight(glm::vec4 position, glm::vec4 direction, float innerCutOff, float outterCutOff, float constant, float linear, float quadric, LightColor color) :
			position{ position }, direction{ direction }, innerCutOff{ innerCutOff }, outterCutOff{ outterCutOff }, constant{ constant }, linear{ linear }, quadric{ quadric }, color{ color } {}

		alignas(16) glm::vec4 position{ 0.0f, 8.0f, 8.0f, 1.0f };
		alignas(16) glm::vec4 direction{ 0.0f, -1.0f, -1.0f, 1.0f };
		float innerCutOff{ 1.0f };
		float outterCutOff{ 12.5f };
		float constant{ 1.0f };
		float linear{ 0.22 };
		float quadric{ 0.2 };
		LightColor color;
	};

	struct Light
	{
		Light() = default;
		Light(glm::vec4 position, glm::vec4 color):
			position{ position }, color{ color } {}
		alignas(16) glm::vec4 position;
		alignas(16) glm::vec4 color;
	};
}