#pragma once
#include <glm\ext\matrix_float4x4.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct WireframeModelDescriptor
	{
		WireframeModelDescriptor() = default;
		WireframeModelDescriptor(glm::vec3 wireframeColor) :
			wireframeColor{ wireframeColor },
			modelMatrix{} {}

		glm::mat4 modelMatrix;
		glm::vec3 wireframeColor;
	};
}