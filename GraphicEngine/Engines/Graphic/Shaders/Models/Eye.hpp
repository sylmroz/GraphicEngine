#pragma once

#include <glm/glm.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct Eye
	{
		Eye() = default;
		Eye(glm::vec3 position) :
			position{ position }{}
		alignas(16) glm::vec3 position;
	};
}