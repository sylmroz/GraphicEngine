#include "GeometryUtils.hpp"
#include <glm\geometric.hpp>
#include <stdexcept>

glm::vec3 GraphicEngine::Core::Math::calculateNormalFromTriangle(const std::array<glm::vec3, 3>& triangle)
{
    glm::vec3 dir1 = triangle[1] - triangle[0];
    glm::vec3 dir2 = triangle[2] - triangle[0];
    glm::vec3 normal = glm::cross(dir1, dir2);
    return glm::normalize(normal);
}

glm::vec3 GraphicEngine::Core::Math::calculateNormalFromPolygon(const std::vector<glm::vec3>& polygon)
{
    if (polygon.size() < 3)
        throw std::runtime_error("Polygon should contain minumm three verctor!");

    if (polygon.size() == 3)
    {
        return calculateNormalFromTriangle({ polygon[0], polygon[1], polygon[2] });
    }

    glm::vec3 normal = glm::vec3(0, 0, 0);
    
    for (uint32_t i{ 0 }; i < polygon.size(); ++i) 
    {
        glm::vec3 current = polygon[i];
        glm::vec3 next = polygon[i % polygon.size()];

        normal.x += (current.y - next.y) * (current.z + next.z);
        normal.y += (current.z - next.z) * (current.x + next.x);
        normal.z += (current.x - next.x) * (current.y + next.y);
    }
    return glm::normalize(normal);
}
