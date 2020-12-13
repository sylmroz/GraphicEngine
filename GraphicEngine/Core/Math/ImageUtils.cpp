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

glm::vec3 GraphicEngine::Core::rgbToHsv(glm::vec3 rgb)
{
	glm::vec3 hsv;
	auto min = std::min({ rgb.r, rgb.g, rgb.b });
	auto max = std::max({ rgb.r, rgb.g, rgb.b });

	hsv.z = max;
	auto delta = max - min;
	if (max != 0)
		hsv.y = delta / max;
	else
	{
		// TODO throw error
	}

	if (rgb.r == max)
		hsv.x = (rgb.g - rgb.b) / delta;
	else if (rgb.g == max)
		hsv.x = 2 + (rgb.b - rgb.r) / delta;
	else
		hsv.x = 4 + (rgb.r - rgb.g) / delta;

	hsv.x *= 60;
	if (hsv.x < 0)
		hsv.x += 360;
	if (!std::isfinite(hsv.x))
		hsv.x = 0;

	return hsv;
}

glm::vec3 GraphicEngine::Core::hsvToRgb(glm::vec3 hsv)
{
	glm::vec3 rgb;
	if (hsv.y == 0)
		return glm::vec3(hsv.z);

	float h = hsv.x / 60;
	int i = std::floorf(h);
	float f = h - i;
	float p = hsv.z * (1 - hsv.y);
	float q = hsv.z * (1 - hsv.y * f);
	float t = hsv.z * (1 - hsv.y * (1 - f));
	switch (i)
	{
	case 1:
		rgb = glm::vec3(hsv.z, t, p);
		break;
	case 2:
		rgb = glm::vec3(q, hsv.z, p);
		break;
	case 3:
		rgb = glm::vec3(p, q, hsv.z);
		break;
	case 4:
		rgb = glm::vec3(t, p, hsv.z);
		break;
	default:
		rgb = glm::vec3(hsv.z, p, q);
		break;
	}

	return rgb;
}

glm::vec3 GraphicEngine::Core::changeContrast(glm::vec3 color, glm::vec3 factor)
{
	return factor * color;
}
