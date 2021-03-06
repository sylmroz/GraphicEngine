#pragma once

#include "IObjectGenerator.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace GraphicEngine::Engines::Graphic
{
	template <typename Vertex>
	class CylinderGenerator : public IObjectGenerator<Vertex, glm::vec3, float, float, float, glm::ivec3, bool, bool, bool, TriangleDirection>
	{
	public:
		virtual std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D, glm::vec3> getObject(
			glm::vec3 center, float radiusBottom, float radiusTop, float height, glm::ivec3 scale,
			bool generateBottom = true, bool generateMiddle = true, bool generateTop = true,
			TriangleDirection triangleDirection = TriangleDirection::Clockwise) override
		{
			float fiStep{ 360.0f / (scale.x + 1) };
			float bottomRadiusStep{ radiusBottom / (scale.y + 1) };
			float topRadiusStep{ radiusTop / (scale.y + 1) };
			float heightStep{ height / (scale.z + 1) };
			float secondRadiusStep{ (radiusBottom - radiusTop) / (scale.z + 1) };
			float boudingBoxRadius{ std::max(radiusBottom, radiusTop) };

			Core::BoudingBox3D boudingBox(glm::vec3(-boudingBoxRadius, 0.0f, -boudingBoxRadius) + center, glm::vec3(boudingBoxRadius, height, boudingBoxRadius) + center);

			std::vector<std::shared_ptr<Vertex>> vertices;
			std::vector<std::shared_ptr<Scene::Face>> faces;
			vertices.reserve(2*(scale.x + 1) * (scale.y + 1) + (scale.x + 1) * (scale.z + 2) + 2);
			faces.reserve(4 * (scale.x * scale.y) + 2 * scale.x * scale.z + 2 * scale.x);

			uint32_t verticesOffset{ 0 };

			if (generateBottom)
			{
				auto v1 = std::make_shared<Vertex>();
				v1->position = center;
				vertices.push_back(v1);
				boudingBox.extendBox(v1->position);

				verticesOffset = vertices.size();

				for (uint32_t y{ 0 }; y < scale.y + 1; ++y)
				{
					float internalRadius{ (y + 1) * bottomRadiusStep };
					for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
					{
						float internalFi{ glm::radians(x * fiStep) };
						auto v2 = std::make_shared<Vertex>();
						v2->position = glm::vec3(internalRadius * std::cos(internalFi), 0.0f, internalRadius * std::sin(internalFi)) + center;
						vertices.push_back(v2);
						boudingBox.extendBox(v2->position);
					}
				}

				for (uint32_t i{ 0 }; i < scale.x + 1; ++i)
				{
					faces.push_back(this->buildFace(verticesOffset - 1, verticesOffset + (i == scale.x ? 1 : i + 2), verticesOffset + i + 1, triangleDirection));
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

				for (uint32_t z{ 0 }; z < scale.z + 2; ++z)
				{
					float internalRadius{ radiusBottom - z * secondRadiusStep };
					for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
					{
						float internalFi{ glm::radians(x * fiStep) };
						auto v2 = std::make_shared<Vertex>();
						v2->position = glm::vec3(internalRadius * std::cos(internalFi), z * heightStep, internalRadius * std::sin(internalFi)) + center;
						vertices.push_back(v2);
						boudingBox.extendBox(v2->position);
					}
				}

				for (uint32_t z{ 0 }; z < scale.z + 1; ++z)
				{
					for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
					{
						uint32_t point = x + (z * (scale.x + 1)) + verticesOffset;
						faces.push_back(this->buildFace(point, (x == scale.x ? point + 1 : point + 2 + scale.x), point + 1 + scale.x, triangleDirection));
						faces.push_back(this->buildFace(point, (x == scale.x ? (point - scale.x) : (point + 1)), (x == scale.x ? point + 1 : point + 2 + scale.x), triangleDirection));
					}
				}
			}

			if (generateTop)
			{
				verticesOffset = vertices.size();

				auto v3 = std::make_shared<Vertex>();
				v3->position = center + glm::vec3(0.0f, height, 0.0f);
				vertices.push_back(v3);
				boudingBox.extendBox(v3->position);

				verticesOffset = vertices.size();

				for (uint32_t y{ 0 }; y < scale.y + 1; ++y)
				{
					float internalRadius{ (y + 1) * topRadiusStep };
					for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
					{
						float internalFi{ glm::radians(x * fiStep) };
						auto v2 = std::make_shared<Vertex>();
						v2->position = glm::vec3(internalRadius * std::cos(internalFi), height, internalRadius * std::sin(internalFi)) + center;
						vertices.push_back(v2);
						boudingBox.extendBox(v2->position);
					}
				}

				for (uint32_t i{ 0 }; i < scale.x + 1; ++i)
				{
					faces.push_back(this->buildFace(verticesOffset - 1, verticesOffset + i + 1, verticesOffset + (i == scale.x ? 1 : i + 2), triangleDirection));
				}

				for (uint32_t y{ 0 }; y < scale.y; ++y)
				{
					for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
					{
						uint32_t point = x + (y * (scale.x + 1)) + verticesOffset;
						faces.push_back(this->buildFace(point, point + 1 + scale.x, (x == scale.x ? point + 1 : point + 2 + scale.x), triangleDirection));
						faces.push_back(this->buildFace(point, (x == scale.x ? point + 1 : point + 2 + scale.x), (x == scale.x ? (point - scale.x) : (point + 1)), triangleDirection));
					}
				}
			}

			return std::make_tuple(vertices, faces, boudingBox, center);
		}
	};
}