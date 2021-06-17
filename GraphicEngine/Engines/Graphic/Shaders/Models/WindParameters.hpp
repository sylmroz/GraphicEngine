#pragma once

#include "../../../../Core/Configuration.hpp"
#include <glm/vec2.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct WindParameters
	{
		WindParameters() = default;
		WindParameters(glm::vec2 direcion, float speed) : 
            direction{ direcion }, speed{ speed } {}
		WindParameters(std::shared_ptr<Core::Configuration> cfg);
		glm::vec2 direction;
        float speed;
	};
}