#pragma once

#include "BoudingBox3D.hpp"

namespace GraphicEngine::Engines::Graphic
{
	class BoudingCube : public BoudingBox3D
	{
	public:
		BoudingCube() = default;

		BoudingCube(glm::vec3 center, float width);

	private:
		float m_width;

		void recalculateLeftAndRight();
	};
}