#pragma once

namespace GraphicEngine::Engines::Graphic::Shaders
{
    struct ShadowRenderingOptions
    {
        bool directional{ true };
        bool point{ true };
        bool spot{ true };
    };

    struct RenderingOptions
    {
        ShadowRenderingOptions shadowRendering;
        bool ambientOcclusion{ true };
        bool globalIllumination{ true };
    };
}