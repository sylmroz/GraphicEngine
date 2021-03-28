#pragma once

#include "IObjectGenerator.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace GraphicEngine::Engines::Graphic
{
	template <typename Vertex>
	class PlaneGenerator : public IObjectGenerator<Vertex, glm::vec2, glm::vec2, glm::ivec2, GeneratingPosition, TriangleDirection>
	{
	public:
		virtual std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D, glm::vec3> getObject(glm::vec2 beginPosition, glm::vec2 endPosition, glm::ivec2 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::Clockwise) override
		{
			std::vector<std::shared_ptr<Vertex>> vertices;
			std::vector<std::shared_ptr<Scene::Face>> faces;

			vertices.reserve((scale.x + 1) * (scale.y + 1));
			faces.reserve(scale.x * scale.y * 2);

			auto from = beginPosition;
			auto to = endPosition;
			
			if (generateFrom == GeneratingPosition::Center)
			{
				from = beginPosition - endPosition;
				to = beginPosition + endPosition;
			}

			auto thick = std::max(scale.x, scale.y) * 0.01f;

			Core::BoudingBox3D boudingBox(glm::vec3(from.x, -thick, from.y), glm::vec3(to.x, thick, to.y));

			glm::vec2 step(to - from);
			step.x /= scale.x;
			step.y /= scale.y;

			// Generate vertices
			for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
			{
				for (uint32_t y{ 0 }; y < scale.y + 1; ++y)
				{
					auto v = std::make_shared<Vertex>();
					v->position = glm::vec3(from + step * glm::vec2(x, y), 0.0f);
					std::swap(v->position.y, v->position.z);
					vertices.push_back(v);
					boudingBox.extendBox(v->position);
				}
			}

			// Generate triangles from vertices.
			//	  
			//     __ __ __ __ 19
			//   3| /| /| /| /|
			//    |/_|/_|/_|/_|
			//   2| /| /| /| /|18
			//    |/_|/_|/_|/_|
			//   1| /| /| /| /|17
			//    |/_|/_|/_|/_|
			//   0   4  8  12 16

			for (uint32_t x{ 0 }; x < scale.x; ++x)
			{
				for (uint32_t y{ 0 }; y < scale.y; ++y)
				{
					uint32_t point = y + (x * (scale.y + 1));
					faces.push_back(this->buildFace(point, point + 1 + scale.y, point + 2 + scale.y, triangleDirection));
					faces.push_back(this->buildFace(point, point + 2 + scale.y, point + 1, triangleDirection));
				}
			}

			return std::make_tuple(vertices, faces, boudingBox, boudingBox.getCenter());
		}
	};
}