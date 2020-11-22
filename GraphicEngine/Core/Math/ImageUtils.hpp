#pragma once
#include <cstdint>
#include <glm/vec3.hpp>

namespace GraphicEngine::Core
{
	uint32_t calculateMipLevels(uint32_t width, uint32_t height);

	glm::vec3 randomColor();
}