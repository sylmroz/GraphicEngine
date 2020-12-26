#pragma once

#include <glm/glm.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct Eye
	{
		Eye() = default;
		Eye(glm::vec4 position) :
			position{ position }{}
		alignas(16) glm::vec4 position;
	};
}