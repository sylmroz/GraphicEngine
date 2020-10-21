#pragma once

#include "IObjectGenerator.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace GraphicEngine::Engines::Graphic
{
	template <typename Vertex>
	class SphereGenerator : public IObjectGenerator<Vertex, glm::vec3, float, glm::ivec2, TriangleDirection>
	{
	public:
		virtual std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getObject(glm::vec3 centerPosition, float radius, glm::ivec2 scale,
			TriangleDirection triangleDirection = TriangleDirection::Clockwise) override
		{
			std::vector<std::shared_ptr<Vertex>> vertices;
			std::vector<std::shared_ptr<Scene::Face>> faces;

			Core::BoudingBox3D boudingBox(glm::vec3(-radius) + centerPosition, glm::vec3(radius) + centerPosition);

			vertices.reserve(2 + ((scale.x + 1) * scale.y));
			faces.reserve(2 * (scale.x + 1) * (scale.y + 1));
			float fiStep = 360.0f / (scale.x + 1);
			float thetaStep = 180.f / (scale.y + 1);

			auto v1 = std::make_shared<Vertex>();
			v1->position = glm::vec3(centerPosition.x, centerPosition.y, centerPosition.z - radius);
			vertices.push_back(v1);

			for (float theta = -180.0f + thetaStep; theta < -0.1f; theta += thetaStep)
			{
				float radTheta = glm::radians(theta);
				float sinTheta = std::sin(radTheta);
				float cosTheta = std::cos(radTheta);
				for (float fi{ 0.0f }; fi < 359.0f; fi += fiStep)
				{
					float radFi = glm::radians(fi);
					float sinFi = std::sin(radFi);
					float cosFi = std::cos(radFi);
					auto v3 = std::make_shared<Vertex>();
					v3->position = glm::vec3(radius * sinTheta * cosFi, radius * sinTheta * sinFi, radius * cosTheta) + centerPosition;
					vertices.push_back(v3);
				}
			}

			auto v2 = std::make_shared<Vertex>();
			v2->position = glm::vec3(centerPosition.x, centerPosition.y, centerPosition.z + radius);
			vertices.push_back(v2);

			for (uint32_t i{ 0 }; i < scale.x + 1; ++i)
			{
				faces.push_back(this->buildFace(0, i + 1, i == scale.x ? 1 : i + 2, triangleDirection));
			}

			for (uint32_t y{ 0 }; y < scale.y - 1; ++y)
			{
				for (uint32_t x{ 0 }; x < scale.x + 1; ++x)
				{
					uint32_t point = x + (y * (scale.x + 1)) + 1;
					faces.push_back(this->buildFace(point, point + 1 + scale.x, (x == scale.x ? point + 1 : point + 2 + scale.x), triangleDirection));
					faces.push_back(this->buildFace(point, (x == scale.x ? point + 1 : point + 2 + scale.x), (x == scale.x ? (point - scale.x) : (point + 1)), triangleDirection));
				}
			}

			uint32_t last = vertices.size() - 1;
			uint32_t offset = last - (scale.x + 1);
			for (uint32_t i{ offset }; i < last - 1; ++i)
			{
				faces.push_back(this->buildFace(i, last, i + 1, TriangleDirection::CounterClockwise));
			}

			faces.push_back(this->buildFace(last -1, last, offset, TriangleDirection::CounterClockwise));

			return std::make_tuple(vertices, faces, boudingBox);
		}
	};
}
