#pragma once
#include <cstdint>
#include <glm/vec3.hpp>

namespace GraphicEngine::Core
{
	uint32_t calculateMipLevels(uint32_t width, uint32_t height);

	glm::vec3 randomColor();

	glm::vec3 rgbToHsv(glm::vec3 rgb);

	glm::vec3 hsvToRgb(glm::vec3 hsv);

	glm::vec3 changeContrast(glm::vec3 color, glm::vec3 factor);
}