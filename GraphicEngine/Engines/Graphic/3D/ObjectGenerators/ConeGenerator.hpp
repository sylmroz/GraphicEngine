#pragma once

#include "IObjectGenerator.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace GraphicEngine::Engines::Graphic
{
	template <typename Vertex>
	class ConeGenerator : public IObjectGenerator<Vertex, glm::vec3, float, float, glm::ivec3, bool, bool, TriangleDirection>
	{
	public:
		virtual std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D, glm::vec3> getObject(glm::vec3 center, float radius, float height, glm::ivec3 scale,
			bool generateBottom = true, bool generateMiddle = true,
			TriangleDirection triangleDirection = TriangleDirection::Clockwise) override
		{
			float fiStep = 360.0f / (scale.x + 1);
			float radiusStep = radius / (scale.y + 1);
			float heightStep = height / (scale.z + 1);
			float secondRadiusStep{ radius / (scale.z + 1) };
			
			Core::BoudingBox3D boudingBox(glm::vec3(-radius, 0.0f, -radius) + center, glm::vec3(radius, height, radius) + center);

			std::vector<std::shared_ptr<Vertex>> vertices;
			std::vector<std::shared_ptr<Scene::Face>> faces;
			vertices.reserve((scale.x + 1) * (scale.y + 1) + (scale.x + 2) * (scale.z + 2));
			faces.reserve(2 * (scale.x * scale.y) + 2 * scale.x * scale.z + 2 * scale.x);

			uint32_t verticesOffset{ 0 };

			if (generateBottom)
			{
				auto v1 = std::make_shared<Vertex>();
				v1->position = center;
				vertices.push_back(v1);

				uint32_t verticesOffset = vertices.size();

				for (uint32_t y{ 0 }; y < scale.y + 1; ++y)
				{
					float internalRadius{ (y + 1) * radiusStep };
					for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
					{
						float internalFi{ glm::radians(x * fiStep) };
						auto v2 = std::make_shared<Vertex>();
						v2->position = glm::vec3(internalRadius * std::cos(internalFi), 0.0f, internalRadius * std::sin(internalFi)) + center;
						vertices.push_back(v2);
					}
				}

				for (uint32_t i{ 0 }; i < scale.x + 1; ++i)
				{
					faces.push_back(this->buildFace(0, i == scale.x ? 1 : i + 2, i + 1, triangleDirection));
				}

				for (uint32_t y{ 0 }; y < scale.y; ++y)
				{
					for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
					{
						uint32_t point = x + (y * (scale.x + 1)) + 1;
						faces.push_back(this->buildFace(point, (x == scale.x ? point + 1 : point + 2 + scale.x), point + 1 + scale.x, triangleDirection));
						faces.push_back(this->buildFace(point, (x == scale.x ? (point - scale.x) : (point + 1)), (x == scale.x ? point + 1 : point + 2 + scale.x), triangleDirection));
					}
				}
			}

			if (generateMiddle)
			{
				verticesOffset = vertices.size();

				for (uint32_t z{ 0 }; z < scale.z + 1; ++z)
				{
					float internalRadius{ radius - z * secondRadiusStep };
					for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
					{
						float internalFi{ glm::radians(x * fiStep) };
						auto v2 = std::make_shared<Vertex>();
						v2->position = glm::vec3(internalRadius * std::cos(internalFi), z * heightStep, internalRadius * std::sin(internalFi)) + center;
						vertices.push_back(v2);
					}
				}

				for (uint32_t z{ 0 }; z < scale.z; ++z)
				{
					for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
					{
						uint32_t point = x + (z * (scale.x + 1)) + verticesOffset;
						faces.push_back(this->buildFace(point, (x == scale.x ? point + 1 : point + 2 + scale.x), point + 1 + scale.x, triangleDirection));
						faces.push_back(this->buildFace(point, (x == scale.x ? (point - scale.x) : (point + 1)), (x == scale.x ? point + 1 : point + 2 + scale.x), triangleDirection));
					}
				}

				uint32_t last = vertices.size() - 1;
				uint32_t offset = last - (scale.x + 1);
				for (uint32_t i{ offset }; i < last - 1; ++i)
				{
					auto v3 = std::make_shared<Vertex>();
					v3->position = center + glm::vec3(0.0f, height, 0.0f);
					vertices.push_back(v3);
					faces.push_back(this->buildFace(i, i + 1, vertices.size() - 1, triangleDirection));
				}

				auto v3 = std::make_shared<Vertex>();
				v3->position = center + glm::vec3(0.0f, height, 0.0f);
				vertices.push_back(v3);
				faces.push_back(this->buildFace(last - 1, offset, vertices.size() - 1, triangleDirection));
			}

			return std::make_tuple(vertices, faces, boudingBox, center);
		}
	};
}