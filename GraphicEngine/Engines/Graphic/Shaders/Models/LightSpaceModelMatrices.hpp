#pragma once

#include <glm/matrix.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct LightSpaceModelMatrices
	{
		LightSpaceModelMatrices() = default;
		LightSpaceModelMatrices(glm::mat4 lightSpace, glm::mat4 model) :
			lightSpace{ lightSpace }, model{ model }
		{}
		alignas(16) glm::mat4 lightSpace;
		alignas(16) glm::mat4 model;
	};
}