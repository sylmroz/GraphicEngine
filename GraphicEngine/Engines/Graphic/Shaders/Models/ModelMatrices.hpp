#pragma once

#include <glm/matrix.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct ModelMartices
	{
		ModelMartices() = default;
		ModelMartices(glm::mat4 modelMatrix, glm::mat4 normalMatrix) :
			modelMatrix{ modelMatrix }, normalMatrix{ normalMatrix }
		{}
		alignas(16) glm::mat4 modelMatrix;
		alignas(16) glm::mat4 normalMatrix;
	};
}