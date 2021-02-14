#pragma once

#include <glm/matrix.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct ModelMatrix
	{
		ModelMatrix() = default;
		ModelMatrix(glm::mat4 model) :
			model{ model }
		{}
		alignas(16) glm::mat4 model;
	};
}