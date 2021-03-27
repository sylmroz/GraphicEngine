#pragma once

#include "ModelMatrices.hpp"

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct SolidColorModelDescriptor
	{
		SolidColorModelDescriptor() = default;
		SolidColorModelDescriptor(glm::mat4 modelMatrix, glm::mat4 normalMatrix):
			modelMatrix{ modelMatrix }, normalMatrix{ normalMatrix }{}
		alignas(16) glm::mat4 modelMatrix;
		alignas(16) glm::mat4 normalMatrix;
	};
}