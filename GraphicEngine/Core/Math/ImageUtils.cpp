#include "ImageUtils.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>

uint32_t GraphicEngine::Core::calculateMipLevels(uint32_t width, uint32_t height)
{
	return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
}

glm::vec3 GraphicEngine::Core::randomColor()
{
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 e2(seed);
	std::uniform_real_distribution<float> dist(0, 1.0f);
	return glm::vec3(dist(e2), dist(e2), dist(e2));
}
