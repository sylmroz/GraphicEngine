#pragma once

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct GrassParameters
	{
		GrassParameters() = default;
		GrassParameters(float thick, float height, int grassPerUnit) :
			thick{ thick }, height{ height }, grassPerUnit{ grassPerUnit } {}
            
		float thick;
        float height;
        int grassPerUnit;
	};
}