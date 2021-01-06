#pragma once
#include <glm\ext\matrix_float4x4.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct WireframeModelDescriptor
	{
		WireframeModelDescriptor() = default;
		WireframeModelDescriptor(glm::vec4 wireframeColor) :
			wireframeColor{ wireframeColor },
			modelMatrix{} {}

		alignas(16) glm::mat4 modelMatrix;
		alignas(16) glm::vec4 wireframeColor;
	};
}