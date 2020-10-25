#pragma once

#include "ObjectGenerators/CuboidGenerator.hpp"
#include "ObjectGenerators/ConeGenerator.hpp"
#include "ObjectGenerators/CylinderGenerator.hpp"
#include "ObjectGenerators/PlaneGenerator.hpp"
#include "ObjectGenerators/SphereGenerator.hpp"

namespace GraphicEngine::Engines::Graphic
{
	template <typename Vertex>
	class ObjectGenerator
	{
	public:
		static std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getPlaneObject(
			glm::vec2 beginPosition, glm::vec2 endPosition, glm::ivec2 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return PlaneGenerator<Vertex>{}.getObject(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::shared_ptr<Scene::Mesh<Vertex>> getPlaneMesh(
			glm::vec2 beginPosition, glm::vec2 endPosition, glm::ivec2 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return PlaneGenerator<Vertex>{}.getMesh(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getPlaneModel(
			glm::vec2 beginPosition, glm::vec2 endPosition, glm::ivec2 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return PlaneGenerator<Vertex>{}.getModel(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getCuboidObject(
			glm::vec3 beginPosition, glm::vec3 endPosition, glm::ivec3 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return CuboidGenerator<Vertex>{}.getObject(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::shared_ptr<Scene::Mesh<Vertex>> getCuboidMesh(glm::vec3 beginPosition, glm::vec3 endPosition, glm::ivec3 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return CuboidGenerator<Vertex>{}.getMesh(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getCuboidModel(glm::vec3 beginPosition, glm::vec3 endPosition, glm::ivec3 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return CuboidGenerator<Vertex>{}.getModel(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getSpheredObject(
			glm::vec3 centerPosition, float radius, glm::ivec2 scale,
			TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return SphereGenerator<Vertex>{}.getObject(centerPosition, radius, scale, triangleDirection);
		}

		static std::shared_ptr<Scene::Mesh<Vertex>> getSphereMesh(glm::vec3 centerPosition, float radius, glm::ivec2 scale,
			TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return SphereGenerator<Vertex>{}.getMesh(centerPosition, radius, scale, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getSphereModel(glm::vec3 centerPosition, float radius, glm::ivec2 scale,
			TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return SphereGenerator<Vertex>{}.getModel(centerPosition, radius, scale, triangleDirection);
		}

		static std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getConeObject(
			glm::vec3 center, float radius, float height, glm::ivec3 scale,
			TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return ConeGenerator<Vertex>{}.getObject(center, radius, scale, triangleDirection);
		}

		static std::shared_ptr<Scene::Mesh<Vertex>> getConeMesh(glm::vec3 center, float radius, float height, glm::ivec3 scale,
			TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return ConeGenerator<Vertex>{}.getMesh(center, radius, scale, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getConeModel(glm::vec3 center, float radius, float height, glm::ivec3 scale,
			TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return ConeGenerator<Vertex>{}.getModel(center, radius, scale, triangleDirection);
		}

		static std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getCylinderObject(
			glm::vec3 center, float radiusBottom, float radiusTop, float height,
			glm::ivec3 scale, TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return CylinderGenerator<Vertex>{}.getObject(center, radiusBottom, radiusTop, height, scale, triangleDirection);
		}

		static std::shared_ptr<Scene::Mesh<Vertex>> getConeMesh(glm::vec3 center, float radiusBottom, float radiusTop, float height,
			glm::ivec3 scale, TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return CylinderGenerator<Vertex>{}.getMesh(center, radiusBottom, radiusTop, height, scale, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getConeModel(glm::vec3 center, float radiusBottom, float radiusTop, float height,
			glm::ivec3 scale, TriangleDirection triangleDirection = TriangleDirection::Clockwise)
		{
			return CylinderGenerator<Vertex>{}.getModel(center, radiusBottom, radiusTop, height, scale, triangleDirection);
		}
	};
}