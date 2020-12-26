#pragma once

#include <glm/matrix.hpp>

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct CameraMatrices
	{
		CameraMatrices() = default;
		CameraMatrices(glm::mat4 view, glm::mat4 projection) :
			view{ view }, projection{ projection }
		{}
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
	};
}