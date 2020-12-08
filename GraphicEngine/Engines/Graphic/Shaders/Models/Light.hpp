#pragma once

#include <glm/glm.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct Light
	{
		Light() = default;
		Light(glm::vec3 position, glm::vec3 color):
			position{ position }, color{ color } {}
		alignas(16) glm::vec3 position;
		alignas(16) glm::vec3 color;
	};
}