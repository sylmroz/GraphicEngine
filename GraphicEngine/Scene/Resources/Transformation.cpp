#include "Transformation.hpp"

glm::mat4 GraphicEngine::Scene::Transformation::getModelMatrix()
{
	if (m_matrixNeedUpdate)
	{
		// Create translate matrix for pivot point
		// We would to make scale and rotate transformations around pivot point
		auto pivotTranslateMatrix = glm::translate(glm::vec3(0.0));

		auto scaleMatrix = glm::scale(pivotTranslateMatrix, m_scale);

		auto rotateXMatrix = glm::rotate(pivotTranslateMatrix, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
		auto rotateYMatrix = glm::rotate(pivotTranslateMatrix, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
		auto rotateZMatrix = glm::rotate(pivotTranslateMatrix, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));

		auto rotateMatrix = rotateZMatrix * rotateYMatrix * rotateXMatrix;

		auto translateMatrix = glm::translate(m_position);

		m_modelMatrix = translateMatrix * rotateMatrix * scaleMatrix;

		if (m_parentTransforamtion != nullptr)
		{
			m_modelMatrix = m_parentTransforamtion->getModelMatrix() * m_modelMatrix;
		}

		m_matrixNeedUpdate = false;
		m_boudingBox.transform(m_modelMatrix);
	}

	return m_modelMatrix;
}

void GraphicEngine::Scene::Transformation::resetTransformation()
{
	m_position = glm::vec3(0.0f);
	m_rotation = glm::vec3(0.0f);
	m_scale = glm::vec3(1.0f);
	m_matrixNeedUpdate = false;
	m_modelMatrix = glm::identity<glm::mat4>();
}
