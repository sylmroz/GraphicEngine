#pragma once

#include <stdint.h>

namespace GraphicEngine::Engines::Graphic::Shaders
{
    struct ShadowRenderingOptions
    {
        uint32_t directional{ 1 };
        uint32_t point{ 1 };
        uint32_t spot{ 1 };
    };

    struct RenderingOptions
    {
        ShadowRenderingOptions shadowRendering;
        uint32_t ambientOcclusion{ 1 };
        uint32_t globalIllumination{ 1 };
    };
}