#pragma once

#include <glm/matrix.hpp>
#include "TypeArray.hpp"

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct LightSpaceMatrix
	{
		LightSpaceMatrix() = default;
		LightSpaceMatrix(glm::mat4 lightSpace) :
			lightSpace{ lightSpace }
		{}
		alignas(16) glm::mat4 lightSpace;
	};

	struct LightSpaceMatrixArray : TypeArray<LightSpaceMatrix> {};
}