#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../../../../Core/Utils/ObjectConverter.hpp"
#include "../../../../Core/Configuration.hpp"
#include <iostream>

namespace GraphicEngine::Engines::Graphic::Shaders
{

	struct LightColor
	{
		LightColor() = default;
		LightColor(std::shared_ptr<Core::Configuration> cfg);

		LightColor(glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 specular);
		
		alignas(16) glm::vec4 diffuse{ 0.5f, 0.5f, 0.5f, 1.0f };
		alignas(16) glm::vec4 ambient{ 0.1f, 0.1f, 0.1f, 1.0f };
		alignas(16) glm::vec4 specular{ 0.5f, 0.5f, 0.5f, 1.0f };
	};

	struct DirectionalLight
	{
		DirectionalLight();
		DirectionalLight(std::shared_ptr<Core::Configuration> cfg);
		DirectionalLight(glm::vec4 direction, LightColor color);

		void calculateLigthSpace();
		
		alignas(16) glm::mat4 lightSpace;
		alignas(16) glm::vec4 direction{ 1.0f, -1.0f, 1.0f, 1.0f };
		alignas(16) LightColor color;
	};

	struct PointLight
	{
		PointLight() = default;
		PointLight(std::shared_ptr<Core::Configuration> cfg);
		PointLight(glm::vec4 position, float constant, float linear, float quadric, LightColor color);

		alignas(16) glm::vec4 position{ 2.0f, 2.0f, 2.0f, 1.0f };
		float constant{ 1.0f };
		float linear{ 0.22 };
		float quadric{ 0.2 };
		LightColor color;
	};

	struct SpotLight
	{
		SpotLight() = default;
		SpotLight(std::shared_ptr<Core::Configuration> cfg);
		SpotLight(glm::vec4 position, glm::vec4 direction, float innerCutOff, float outterCutOff, float constant, float linear, float quadric, LightColor color);

		alignas(16) glm::vec4 position{ 0.0f, 8.0f, 8.0f, 1.0f };
		alignas(16) glm::vec4 direction{ 0.0f, -1.0f, -1.0f, 1.0f };
		float innerCutOff{ glm::cos(glm::radians(1.0f)) };
		float outterCutOff{ glm::cos(glm::radians(12.5f)) };
		float constant{ 1.0f };
		float linear{ 0.014f };
		float quadric{ 0.007f };
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