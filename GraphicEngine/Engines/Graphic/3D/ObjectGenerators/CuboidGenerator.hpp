#pragma once

#include "IObjectGenerator.hpp"
#include <glm/vec3.hpp>

namespace GraphicEngine::Engines::Graphic
{
	template <typename Vertex>
	class CuboidGenerator : public IObjectGenerator<Vertex, glm::vec3, glm::vec3, glm::ivec3, GeneratingPosition, TriangleDirection>
	{
	public:
		virtual std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getObject(glm::vec3 beginPosition, glm::vec3 endPosition, glm::ivec3 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::Clockwise) override
		{
			auto verticesSize = 2 * (((scale.x + 1) * (scale.z + 1)) + ((scale.x + 1) * (scale.y - 1)) + ((scale.y - 1) * (scale.z - 1)));
			std::vector<std::shared_ptr<Vertex>> vertices;
			std::vector<std::shared_ptr<Scene::Face>> faces;

			vertices.reserve(verticesSize);
			faces.reserve(4 * scale.x * scale.y * scale.z);

			auto from = beginPosition;
			auto to = endPosition;

			if (generateFrom == GeneratingPosition::Center)
			{
				from = beginPosition - endPosition;
				to = beginPosition + endPosition;
			}

			Core::BoudingBox3D boudingBox(from, to);

			glm::vec3 step(to - from);
			step.x /= scale.x;
			step.y /= scale.y;
			step.z /= scale.z;

			// first wall x,z
			for (uint32_t z{ 0 }; z < scale.z + 1; ++z)
			{
				for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
				{
					auto v = std::make_shared<Vertex>();
					v->position = glm::vec3(from + step * glm::vec3(x, 0.0f, z));
					vertices.push_back(v);
				}
			}

			// second wall x,z
			for (uint32_t z{ 0 }; z < scale.z + 1; ++z)
			{
				for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
				{
					auto v = std::make_shared<Vertex>();
					v->position = glm::vec3(from + step * glm::vec3(x, 0.0f, z));
					v->position.y = to.y;
					vertices.push_back(v);
				}
			}

			// first wall x,y
			for (uint32_t y{ 1 }; y < scale.y; ++y)
			{
				for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
				{
					auto v = std::make_shared<Vertex>();
					v->position = glm::vec3(from + step * glm::vec3(x, y, 0.0f));
					vertices.push_back(v);
				}
			}

			// second wall x,y
			for (uint32_t y{ 1 }; y < scale.y; ++y)
			{
				for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
				{
					auto v = std::make_shared<Vertex>();
					v->position = glm::vec3(from + step * glm::vec3(x, y, 0.0f));
					v->position.z = to.z;
					vertices.push_back(v);
				}
			}

			// first wall y,z
			for (uint32_t z{ 1 }; z < scale.z; ++z)
			{
				for (uint32_t y{ 1 }; y < scale.y; ++y)
				{
					auto v = std::make_shared<Vertex>();
					v->position = glm::vec3(from + step * glm::vec3(0.0f, y, z));
					vertices.push_back(v);
				}
			}

			// second wall y,z
			for (uint32_t z{ 1 }; z < scale.z; ++z)
			{
				for (uint32_t y{ 1 }; y < scale.y; ++y)
				{
					auto v = std::make_shared<Vertex>();
					v->position = glm::vec3(from + step * glm::vec3(0.0f, y, z));
					v->position.x = to.x;
					vertices.push_back(v);
				}
			}

			return std::make_tuple(vertices, faces, boudingBox);
		}
	};
}