#pragma once

#include <glm/vec4.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct LightPositionFarPlane
	{
		LightPositionFarPlane() = default;
		LightPositionFarPlane(glm::vec4 lightPosFarPlane) :
			lightPosFarPlane{ lightPosFarPlane }
		{}
		alignas(16) glm::vec4 lightPosFarPlane;
	};

    struct LightPositionFarPlaneArray : TypeArray<LightPositionFarPlane> {};
}