#pragma once

#include <glm/matrix.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct ModelMartices
	{
		ModelMartices() = default;
		ModelMartices(glm::mat4 modelMatrix, glm::mat3 normalMatrix) :
			modelMatrix(modelMatrix), normalMatrix(normalMatrix)
		{}
		glm::mat4 modelMatrix;
		glm::mat3 normalMatrix;
	};
}