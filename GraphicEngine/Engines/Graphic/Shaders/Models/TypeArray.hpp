#pragma once

#include <vector>

namespace GraphicEngine::Engines::Graphic::Shaders
{
    template <typename T>
    struct TypeArray
    {
        using type_base = T;
        TypeArray() = default;
        TypeArray(const std::vector<type_base>& data)
        {
            this->data = data;
        }
        std::vector<type_base> data;
    };
}