#pragma once

#include <glm/glm.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct Light
	{
		Light() = default;
		Light(glm::vec4 position, glm::vec4 color):
			position{ position }, color{ color } {}
		alignas(16) glm::vec4 position;
		alignas(16) glm::vec4 color;
	};
}