#pragma once

#include "../BoundingBox.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <algorithm>

#undef min
#undef max

namespace GraphicEngine::Core
{
	class BoudingBox3D : public BoundingBox<BoudingBox3D, glm::vec3>
	{
	public:
		BoudingBox3D();
		BoudingBox3D(glm::vec3 left, glm::vec3 right);

		void recalculate(glm::vec3 p);

		void transform(glm::mat4 modelMatrix);

		bool isPointInside(glm::vec3 point);

		void applyTransformation();

		void operator=(BoudingBox3D boudingBox)
		{
			m_left = boudingBox.m_left;
			m_right = boudingBox.m_right;
			m_baseLeft = boudingBox.m_baseLeft;
			m_baseRight = boudingBox.m_baseRight;
			m_center = boudingBox.m_center;
		}

	protected:
		glm::vec3 m_baseLeft;
		glm::vec3 m_baseRight;
	};
}