#pragma once

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct GrassParameters
	{
		GrassParameters() = default;
		GrassParameters(float thick, float height, float stiffness) :
			thick{ thick }, height{ height }, stiffness{ stiffness } {}
            
		float thick;
        float height;
        float stiffness;
	};
}