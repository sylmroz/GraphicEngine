#pragma once

#include "../../../Core/Configuration.hpp"
#include "../../../Core/Utils/ObjectConverter.hpp"
#include "ObjectGenerators/ConeGenerator.hpp"
#include "ObjectGenerators/CuboidGenerator.hpp"
#include "ObjectGenerators/CylinderGenerator.hpp"
#include "ObjectGenerators/PlaneGenerator.hpp"
#include "ObjectGenerators/SphereGenerator.hpp"

#include <magic_enum.hpp>

namespace GraphicEngine::Engines::Graphic
{
	enum class GeneratorType
	{
		Cone,
		Cuboid,
		Cylinder,
		Plane,
		Sphere
	};

	enum class Type
	{
		Object,
		Mesh,
		Model,
	};

	template <typename Vertex>
	class ObjectGenerator
	{
	public:
		static std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getPlaneObject(
			glm::vec2 beginPosition, glm::vec2 endPosition, glm::ivec2 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return PlaneGenerator<Vertex>{}.getObject(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::shared_ptr<Scene::Mesh<Vertex>> getPlaneMesh(
			glm::vec2 beginPosition, glm::vec2 endPosition, glm::ivec2 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return PlaneGenerator<Vertex>{}.getMesh(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getPlaneModel(
			glm::vec2 beginPosition, glm::vec2 endPosition, glm::ivec2 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return PlaneGenerator<Vertex>{}.getModel(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getPlaneModel(std::shared_ptr<Core::Configuration> properties)
		{
			auto begin = Core::Utils::Converter::fromArrayToObject<glm::vec2, std::vector<float>, 2>(properties->getProperty<std::vector<float>>("begin"));
			auto end = Core::Utils::Converter::fromArrayToObject<glm::vec2, std::vector<float>, 2>(properties->getProperty<std::vector<float>>("end"));
			auto scale = Core::Utils::Converter::fromArrayToObject<glm::ivec2, std::vector<int>, 2>(properties->getProperty<std::vector<int>>("scale"));
			auto generateFrom = magic_enum::enum_cast<GeneratingPosition>(properties->getProperty<std::string>("generating position"));
			auto triangleDirection = magic_enum::enum_cast<TriangleDirection>(properties->getProperty<std::string>("triangle direction"));
			return PlaneGenerator<Vertex>{}.getModel(begin, end, scale, generateFrom.value(), triangleDirection.value());
		}

		static std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getCuboidObject(
			glm::vec3 beginPosition, glm::vec3 endPosition, glm::ivec3 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return CuboidGenerator<Vertex>{}.getObject(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::shared_ptr<Scene::Mesh<Vertex>> getCuboidMesh(glm::vec3 beginPosition, glm::vec3 endPosition, glm::ivec3 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return CuboidGenerator<Vertex>{}.getMesh(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getCuboidModel(glm::vec3 beginPosition, glm::vec3 endPosition, glm::ivec3 scale,
			GeneratingPosition generateFrom = GeneratingPosition::Corner, TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return CuboidGenerator<Vertex>{}.getModel(beginPosition, endPosition, scale, generateFrom, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getCuboidModel(std::shared_ptr<Core::Configuration> properties)
		{
			auto begin = Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>,3>(properties->getProperty<std::vector<float>>("begin"));
			auto end = Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(properties->getProperty<std::vector<float>>("end"));
			auto scale = Core::Utils::Converter::fromArrayToObject<glm::ivec3, std::vector<int>, 3>(properties->getProperty<std::vector<int>>("scale"));
			auto generateFrom = magic_enum::enum_cast<GeneratingPosition>(properties->getProperty<std::string>("generating position"));
			auto triangleDirection = magic_enum::enum_cast<TriangleDirection>(properties->getProperty<std::string>("triangle direction"));
			return CuboidGenerator<Vertex>{}.getModel(begin, end, scale, generateFrom.value(), triangleDirection.value());
		}

		static std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getSpheredObject(
			glm::vec3 centerPosition, float radius, glm::ivec2 scale,
			TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return SphereGenerator<Vertex>{}.getObject(centerPosition, radius, scale, triangleDirection);
		}

		static std::shared_ptr<Scene::Mesh<Vertex>> getSphereMesh(glm::vec3 centerPosition, float radius, glm::ivec2 scale,
			TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return SphereGenerator<Vertex>{}.getMesh(centerPosition, radius, scale, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getSphereModel(glm::vec3 center, float radius, glm::ivec2 scale,
			TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return SphereGenerator<Vertex>{}.getModel(center, radius, scale, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getSphereModel(std::shared_ptr<Core::Configuration> properties)
		{
			auto center = Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(properties->getProperty<std::vector<float>>("center"));
			auto radius = properties->getProperty<float>("radius");
			auto scale = Core::Utils::Converter::fromArrayToObject<glm::ivec2, std::vector<int>, 2>(properties->getProperty<std::vector<int>>("scale"));
			auto triangleDirection = magic_enum::enum_cast<TriangleDirection>(properties->getProperty<std::string>("triangle direction"));
			return SphereGenerator<Vertex>{}.getModel(center, radius, scale, triangleDirection.value());
		}

		static std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getConeObject(
			glm::vec3 center, float radius, float height, glm::ivec3 scale,
			bool generateBottom = true, bool generateMiddle = true,
			TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return ConeGenerator<Vertex>{}.getObject(center, radius, height, scale, generateBottom, generateMiddle, triangleDirection);
		}

		static std::shared_ptr<Scene::Mesh<Vertex>> getConeMesh(glm::vec3 center, float radius, float height, glm::ivec3 scale,
			bool generateBottom = true, bool generateMiddle = true,
			TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return ConeGenerator<Vertex>{}.getMesh(center, radius, height, scale, generateBottom, generateMiddle, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getConeModel(glm::vec3 center, float radius, float height, glm::ivec3 scale,
			bool generateBottom = true, bool generateMiddle = true,
			TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return ConeGenerator<Vertex>{}.getModel(center, radius, height, scale, generateBottom, generateMiddle, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getConeModel(std::shared_ptr<Core::Configuration> properties)
		{
			auto center = Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(properties->getProperty<std::vector<float>>("center"));
			auto height = properties->getProperty<float>("height");
			auto radius = properties->getProperty<float>("radius");
			auto scale = Core::Utils::Converter::fromArrayToObject<glm::ivec3, std::vector<int>, 3>(properties->getProperty<std::vector<int>>("scale"));
			auto generateBottom = properties->getProperty<bool>("bottom");
			auto generateMiddle = properties->getProperty<bool>("middle");
			auto triangleDirection = magic_enum::enum_cast<TriangleDirection>(properties->getProperty<std::string>("triangle direction"));
			return ConeGenerator<Vertex>{}.getModel(center, radius, height, scale, generateBottom, generateMiddle, triangleDirection.value());
		}

		static std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D> getCylinderObject(
			glm::vec3 center, float radiusBottom, float radiusTop, float height, glm::ivec3 scale,
			bool generateBottom = true, bool generateMiddle = true, bool generateTop = true,
			TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return CylinderGenerator<Vertex>{}.getObject(center, radiusBottom, radiusTop, height, scale, generateBottom, generateMiddle, generateTop, triangleDirection);
		}

		static std::shared_ptr<Scene::Mesh<Vertex>> getCylinderMesh(glm::vec3 center, float radiusBottom, float radiusTop, float height, glm::ivec3 scale,
			bool generateBottom = true, bool generateMiddle = true, bool generateTop = true,
			TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return CylinderGenerator<Vertex>{}.getMesh(center, radiusBottom, radiusTop, height, scale, generateBottom, generateMiddle, generateTop, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getCylinderModel(glm::vec3 center, float radiusBottom, float radiusTop, float height, glm::ivec3 scale,
			bool generateBottom = true, bool generateMiddle = true, bool generateTop = true,
			TriangleDirection triangleDirection = TriangleDirection::CounterClockwise)
		{
			return CylinderGenerator<Vertex>{}.getModel(center, radiusBottom, radiusTop, height, scale, generateBottom, generateMiddle, generateTop, triangleDirection);
		}

		static std::shared_ptr<Scene::Model<Vertex>> getCylinderModel(std::shared_ptr<Core::Configuration> properties)
		{
			auto center = Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(properties->getProperty<std::vector<float>>("center"));
			auto height = properties->getProperty<float>("height");
			auto radiusBottom = properties->getProperty<float>("bottom radius");
			auto radiusTop = properties->getProperty<float>("top radius");
			auto scale = Core::Utils::Converter::fromArrayToObject<glm::ivec3, std::vector<int>, 3>(properties->getProperty<std::vector<int>>("scale"));
			auto generateBottom = properties->getProperty<bool>("bottom");
			auto generateMiddle = properties->getProperty<bool>("middle");
			auto generateTop = properties->getProperty<bool>("top");
			auto triangleDirection = magic_enum::enum_cast<TriangleDirection>(properties->getProperty<std::string>("triangle direction"));
			return CylinderGenerator<Vertex>{}.getModel(center, radiusBottom, radiusTop, height, scale, generateBottom, generateMiddle, generateTop, triangleDirection.value());
		}

		static std::shared_ptr<Scene::Model<Vertex>> generateModel(std::shared_ptr<Core::Configuration> properties, GeneratorType type)
		{
			return modelGenerators[type](properties);
		}

	private:
		static inline std::map<GeneratorType, std::function<std::shared_ptr<Scene::Model<Vertex>>(std::shared_ptr<Core::Configuration>)>> modelGenerators = {
			{ GeneratorType::Plane, [&](std::shared_ptr<Core::Configuration> properties) { return getPlaneModel(properties); } },
			{ GeneratorType::Cuboid, [&](std::shared_ptr<Core::Configuration> properties) { return getCuboidModel(properties); } },
			{ GeneratorType::Sphere, [&](std::shared_ptr<Core::Configuration> properties) { return getSphereModel(properties); } },
			{ GeneratorType::Cylinder, [&](std::shared_ptr<Core::Configuration> properties) { return getCylinderModel(properties); } },
			{ GeneratorType::Cone, [&](std::shared_ptr<Core::Configuration> properties) { return getConeModel(properties); } }
		};
	};
}