#pragma once

#include <glm/glm.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct Light
	{
		glm::vec3 lightPosition;
		glm::vec3 eyePosition;
	};
}