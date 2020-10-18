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

			// Vertices

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

			// Faces

			// first wall x,z
			for (uint32_t z{ 0 }; z < scale.z; ++z)
			{
				for (uint32_t x{ 0 }; x < scale.x; ++x)
				{
					uint32_t point = x + (z * (scale.x + 1));
					if (triangleDirection == TriangleDirection::CounterClockwise)
					{
						faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 1 + scale.x, point + 2 + scale.x })));
						faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 2 + scale.x, point + 1 })));
					}

					else
					{
						faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 1, point + 2 + scale.x })));
						faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 2 + scale.x, point + 1 + scale.x })));
					}
				}
			}

			// second wall x,z
			for (uint32_t z{ 0 }; z < scale.z; ++z)
			{
				auto offset = (scale.x + 1) * (scale.z + 1);
				for (uint32_t x{ 0 }; x < scale.x; ++x)
				{
					uint32_t point = x + (z * (scale.x + 1)) + offset;
					if (triangleDirection == TriangleDirection::CounterClockwise)
					{
						faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 1, point + 2 + scale.x })));
						faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 2 + scale.x, point + 1 + scale.x })));
					}

					else
					{
						faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 1 + scale.x, point + 2 + scale.x })));
						faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 2 + scale.x, point + 1 })));
					}
				}
			}

			// first wall x,y
			for (uint32_t y{ 0 }; y < scale.y; ++y)
			{
				auto offset = 2 * (scale.x + 1) * (scale.z + 1);
				for (uint32_t x{ 0 }; x < scale.x; ++x)
				{
					uint32_t point = x + (y * (scale.x + 1)) + offset;
					if (triangleDirection == TriangleDirection::CounterClockwise)
					{
						if (y == 0)
						{
							uint32_t boundary = point - (scale.x + 1) * (scale.z + 1);
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ boundary,  point + 1, point })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ boundary, boundary + 1, point + 1 })));
						}

						else if (y == scale.y - 1)
						{
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, x + 1, x })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 1, x + 1 })));

						}

						else
						{
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 2 + scale.x, point + 1 + scale.x })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 1, point + 2 + scale.x })));
						}
					}

					else
					{
						if (y == 0)
						{
							uint32_t boundary = point - (scale.x + 1) * (scale.z + 1);
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ boundary, point, point + 1 })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ boundary, point + 1, boundary + 1 })));
						}

						else if (y == scale.y - 1)
						{
							point -= (scale.x + 1);
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, x, x + 1 })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, x + 1, point + 1 })));
						}

						else
						{
							point -= (scale.x + 1);
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 1 + scale.x, point + 2 + scale.x })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 2 + scale.x, point + 1 })));
						}
					}
				}
			}

			// second wall x,y
			for (uint32_t y{ 0 }; y < scale.y; ++y)
			{
				auto baseOffset = (2 * (scale.x + 1) * (scale.z + 1)) + (scale.x + 1) * (scale.y - 1);
				for (uint32_t x{ 0 }; x < scale.x; ++x)
				{
					uint32_t point = x + (y * (scale.x + 1)) + baseOffset;
					if (triangleDirection == TriangleDirection::CounterClockwise)
					{
						if (y == 0)
						{
							uint32_t offset = (scale.x + 1) * scale.z;
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ offset + x, point + 1 + scale.x, point + 2 + scale.x })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ offset, point + 2 + scale.x, offset + 1 })));
						}

						else if (y == scale.y - 1)
						{
							uint32_t offset = (2*(scale.x + 1) * (scale.z + 1)) - scale.z;
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ offset + x, point + 1, point + 2 })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ offset, point + 2, offset + 1 })));
						}

						else
						{
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 1 + scale.x, point + 2 + scale.x })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 2 + scale.x, point + 1 })));
						}
					}

					else
					{
						if (y == 0)
						{
							uint32_t offset = (scale.x + 1) * scale.z;
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ offset + x, point, point + 1 })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ offset + x, point + 1, offset + x + 1 })));
						}

						else if (y == scale.y - 1)
						{
							point -= (scale.x + 1);
							uint32_t offset = (2 * (scale.x + 1) * (scale.z + 1)) - (scale.x + 1);
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, offset + x + 1, point + 1 })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, offset + x, offset + x + 1 })));
						}

						else
						{
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 1, point + 2 + scale.x })));
							faces.push_back(std::make_shared<Scene::Face>(Scene::Face({ point, point + 2 + scale.x, point + 1 + scale.x })));
						}
					}
				}
			}

			// first wall y,z
			for (uint32_t z{ 0 }; z < scale.z; ++z)
			{
				for (uint32_t y{ 0 }; y < scale.y; ++y)
				{
				}
			}

			// second wall y,z
			for (uint32_t z{ 0 }; z < scale.z; ++z)
			{
				for (uint32_t y{ 0 }; y < scale.y; ++y)
				{
				}
			}

			return std::make_tuple(vertices, faces, boudingBox);
		}
	};
}