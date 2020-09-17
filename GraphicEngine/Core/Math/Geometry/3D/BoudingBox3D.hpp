#pragma once

#include "../BoundingBox.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <algorithm>

#undef min
#undef max

namespace GraphicEngine::Engines::Graphic
{
	class BoudingBox3D : public BoundingBox<BoudingBox3D, glm::vec3>
	{
	public:
		BoudingBox3D() = default;
		BoudingBox3D(glm::vec3 left, glm::vec3 right);
	
		void recalculate(glm::vec3 p);

		void transform(glm::mat4 modelMatrix);

	protected:
		glm::vec3 m_baseLeft;
		glm::vec3 m_baseRight;
	};
}