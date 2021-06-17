#include "ArrowDirection.hpp"

GraphicEngine::GUI::ArrowDirection3D::ArrowDirection3D(std::string label, glm::vec3 initialDirection, glm::vec4 arrowColor):
	ArrowDirection<glm::vec3>{ label, initialDirection, arrowColor }
{
	m_direction = initialDirection;
}

void GraphicEngine::GUI::ArrowDirection3D::copyTempDirToOriginalDir()
{
	direction = m_direction;
}

GraphicEngine::GUI::ArrowDirection2D::ArrowDirection2D(std::string label, glm::vec2 initialDirection, glm::vec4 arrowColor):
	ArrowDirection<glm::vec2>{ label, initialDirection, arrowColor }
{
	m_direction = glm::vec3{ initialDirection.x, initialDirection.y, 0.0f };
}

void GraphicEngine::GUI::ArrowDirection2D::copyTempDirToOriginalDir()
{
	direction = glm::vec2{ m_direction.x, m_direction.y };
	m_direction.z = 0;
}
