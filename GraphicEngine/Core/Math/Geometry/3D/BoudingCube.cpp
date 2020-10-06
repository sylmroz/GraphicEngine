#include "BoudingCube.hpp"

GraphicEngine::Core::BoudingCube::BoudingCube(glm::vec3 center, float width)
{
	m_center = center;
	m_width = width;
	recalculateLeftAndRight();
}

void GraphicEngine::Core::BoudingCube::recalculateLeftAndRight()
{
	m_left = m_center - m_width;
	m_right = m_center + m_width;
}
