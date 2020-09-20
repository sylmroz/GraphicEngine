#pragma once

#include "IObjectGenerator.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace GraphicEngine::Engines::Graphic::Generators
{
	template <typename Vertex>
	class PlaneGenerator : public IObjectGenerator<Vertex, glm::vec2, glm::vec2, glm::ivec2, GeneratingPosition, TriangleDirection>
	{
	public:
		virtual std::tuple<std::vector<Vertex>, std::vector<Scene::Face>> getObject(glm::vec2 beginPosition, glm::vec2 endPosition, glm::ivec2 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::Clockwise) override
		{
			std::vector<Vertex> vertices;
			std::vector<Scene::Face> faces;

			vertices.reserve((scale.x + 1) * (scale.y + 1));
			faces.reserve(scale.x * scale.y * 2);

			auto from = beginPosition;
			auto to = endPosition;
			
			if (generateFrom == GeneratingPosition::Center)
			{
				from = beginPosition - endPosition;
				to = beginPosition + endPosition;
			}

			glm::vec2 step(to - from);
			step.x /= scale.x;
			step.y /= scale.y;

			for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
			{
				for (uint32_t y{ 0 }; y < scale.y + 1; ++y)
				{
					Vertex v;
					v.position = glm::vec3(from + step * glm::vec2(x, y), 0.0f);
					vertices.push_back(v);
				}
			}

			for (uint32_t x{ 0 }; x < scale.x; ++x)
			{
				for (uint32_t y{ 0 }; y < scale.y; ++y)
				{
					uint32_t point = y + (x * (scale.y + 1));
					if (triangleDirection == TriangleDirection::Clockwise)
					{	
						faces.push_back(Scene::Face({ point, point + 1, point + 2 + scale.y }));
						faces.push_back(Scene::Face({ point, point + 2 + scale.y, point + 1 + scale.y }));
					}

					else
					{
						faces.push_back(Scene::Face({ point, point + 1 + scale.y, point + 2 + scale.y }));
						faces.push_back(Scene::Face({ point, point + 2 + scale.y, point + 1 }));
					}
				}
			}

			return std::make_tuple(vertices, faces);
		}
	};
}