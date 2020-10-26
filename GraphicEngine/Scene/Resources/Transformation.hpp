#pragma once

#include "../../Core/Math/Geometry/3D/BoudingBox3D.hpp"
#include <glm\geometric.hpp>
#include <glm/gtx/transform.hpp>

namespace GraphicEngine::Scene
{
	class Transformation : public std::enable_shared_from_this<Transformation>
	{
	public:

		void setParent(std::weak_ptr<Transformation> parent)
		{
			m_parentTransforamtion = parent;
		}

		std::weak_ptr<Transformation> getParent()
		{
			return m_parentTransforamtion;
		}

		virtual void applyTransformation() = 0;

		void setPivotPoint(glm::vec3 pivotPoint)
		{
			m_pivotPoint = pivotPoint;
		}

		glm::vec3 getPivotPoint()
		{
			return m_pivotPoint;
		}

		virtual void setPosition(glm::vec3 position)
		{
			m_position = position;
			m_matrixNeedUpdate = true;
		}

		glm::vec3 getPosition()
		{
			return m_position;
		}

		virtual void setScale(glm::vec3 scale)
		{
			m_scale = scale;
			m_matrixNeedUpdate = true;
		}

		virtual void setScale(float scale)
		{
			m_scale = glm::vec3(scale);
			m_matrixNeedUpdate = true;
		}

		glm::vec3 getScale()
		{
			return m_scale;
		}

		virtual void setRotate(glm::vec3 rotate)
		{
			m_rotation = rotate;
			m_matrixNeedUpdate = true;
		}

		glm::vec3 getRotate()
		{
			return m_rotation;
		}

		glm::mat4 getModelMatrix()
		{
			if (m_matrixNeedUpdate)
			{
				m_modelMatrix = glm::mat4(1.0f);
				m_modelMatrix = glm::translate(m_modelMatrix, m_pivotPoint);

				m_modelMatrix = glm::scale(m_modelMatrix, m_scale);

				m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
				m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
				m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));

				m_modelMatrix = glm::translate(m_modelMatrix, m_position);
				
				if (auto parentTransform = m_parentTransforamtion.lock())
				{
					m_modelMatrix = parentTransform->getModelMatrix() * m_modelMatrix;
				}

				m_matrixNeedUpdate = false;
			}

			return m_modelMatrix;
		}
	protected:
		glm::vec3 m_pivotPoint; // average of vertex positions

		glm::vec3 m_position{ 0.0f,0.0f,0.0f };
		glm::vec3 m_rotation{ 0.0f,0.0f,0.0f };
		glm::vec3 m_scale{ 1.0f,1.0f,1.0f };
		bool m_matrixNeedUpdate{ true };
		glm::mat4 m_modelMatrix = glm::mat4(1.0f);

		Core::BoudingBox3D m_boudingBox;

		std::weak_ptr<Transformation> m_parentTransforamtion;
	};
}