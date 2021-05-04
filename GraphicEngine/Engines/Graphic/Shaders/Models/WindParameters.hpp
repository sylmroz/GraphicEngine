#pragma once

#include <glm/vec2.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct WindParameters
	{
		WindParameters() = default;
		WindParameters(glm::vec2 direcion, float strength, float speed) : 
            direcion{ direcion }, strength{ strength }, speed{ speed } {}
		glm::vec2 direcion;
        float strength;
        float speed;
	};
}