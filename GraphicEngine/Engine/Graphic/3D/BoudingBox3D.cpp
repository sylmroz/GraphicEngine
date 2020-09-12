#include "BoudingBox3D.hpp"

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
