#include "BoudingBox3D.hpp"
#include <glm/vec4.hpp>

GraphicEngine::Core::BoudingBox3D::BoudingBox3D()
{
	m_left = glm::vec3(10000000.0f);
	m_right = glm::vec3(-10000000.0f);
}

GraphicEngine::Core::BoudingBox3D::BoudingBox3D(glm::vec3 left, glm::vec3 right) :
	BoundingBox{ left, right },
	m_baseLeft{left},
	m_baseRight{right}
{
}

void GraphicEngine::Core::BoudingBox3D::recalculate(glm::vec3 p)
{
	m_left.x = std::min(m_left.x, p.x);
	m_left.y = std::min(m_left.y, p.y);
	m_left.z = std::min(m_left.z, p.z);

	m_right.x = std::max(m_right.x, p.x);
	m_right.y = std::max(m_right.y, p.y);
	m_right.z = std::max(m_right.z, p.z);

	m_baseLeft = m_left;
	m_baseRight = m_right;
}

void GraphicEngine::Core::BoudingBox3D::transform(glm::mat4 modelMatrix)
{
	auto p1 =  glm::vec3(modelMatrix * glm::vec4(m_baseLeft, 1.0f));
	auto p2 = glm::vec3(modelMatrix * glm::vec4(m_baseRight, 1.0f));

	BoudingBox3D boudingBox(p1, p2);
	m_left = boudingBox.m_left;
	m_right = boudingBox.m_right;
}

bool GraphicEngine::Core::BoudingBox3D::isPointInside(glm::vec3 point)
{
	return ((point.x >= m_left.x && point.x <= m_right.x) &&
		(point.y >= m_left.y && point.y <= m_right.y) &&
		(point.z >= m_left.z && point.z <= m_right.z));
}

void GraphicEngine::Core::BoudingBox3D::applyTransformation()
{
	m_baseLeft = m_left;
	m_baseRight = m_right;
}
