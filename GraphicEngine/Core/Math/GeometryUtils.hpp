#pragma once
#include <glm/vec3.hpp>
#include <array>
#include <vector>

namespace GraphicEngine::Core::Math
{
	glm::vec3 calculateNormalFromTriangle(const std::array<glm::vec3, 3>& triangle);

	glm::vec3 calculateNormalFromPolygon(const std::vector<glm::vec3>& polygon);

	glm::vec3 generateTangent(glm::vec3 normal);

	glm::vec3 generateBitangent(glm::vec3 tangent, glm::vec3 normal);
}