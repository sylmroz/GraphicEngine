#include "ImageUtils.hpp"
#include <algorithm>
#include <cmath>

uint32_t GraphicEngine::Core::calculateMipLevels(uint32_t width, uint32_t height)
{
	return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
}
