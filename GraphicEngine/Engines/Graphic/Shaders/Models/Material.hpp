#pragma once

#include "../../../../Core/Configuration.hpp"
#include <glm/vec4.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
    struct Material
    {
        Material() = default;
		Material(std::shared_ptr<Core::Configuration> cfg);
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
    };
}