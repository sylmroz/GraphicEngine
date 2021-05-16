#pragma once

#include <glm/vec4.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
    struct GrassColor 
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        glm::vec4 translucent;
    };

	struct GrassMaterial
	{
		GrassMaterial() = default;
		GrassMaterial(GrassColor front, GrassColor back, float shininess) :
			front{ front }, back{ back }, shininess{ shininess } {}
            
		GrassColor front;
        GrassColor back;
        float shininess;
	};
}