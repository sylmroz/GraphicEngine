#include "BoudingBox3D.hpp"
#include <glm/vec4.hpp>

GraphicEngine::Engines::Graphic::BoudingBox3D::BoudingBox3D(glm::vec3 left, glm::vec3 right):
	BoundingBox(left, right)
{
}

void GraphicEngine::Engines::Graphic::BoudingBox3D::recalculate(glm::vec3 p)
{
	m_left.x = std::min(m_left.x, p.x);
	m_left.y = std::min(m_left.y, p.y);
	m_left.z = std::min(m_left.z, p.z);

	m_right.x = std::max(m_right.x, p.x);
	m_right.y = std::max(m_right.y, p.y);
	m_right.z = std::max(m_right.z, p.z);
}

void GraphicEngine::Engines::Graphic::BoudingBox3D::transform(glm::mat4 modelMatrix)
{
	auto p1 =  glm::vec3(modelMatrix * glm::vec4(m_left, 1.0f));
	auto p2 = glm::vec3(modelMatrix * glm::vec4(m_right, 1.0f));

	// TODO
}
