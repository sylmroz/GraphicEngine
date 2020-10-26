#pragma once

#include "../../../../Scene/Resources/Model.hpp"

namespace GraphicEngine::Engines::Graphic
{
	enum class GeneratingPosition
	{
		Corner,
		Center
	};

	enum class TriangleDirection
	{
		Clockwise,
		CounterClockwise
	};
}

namespace GraphicEngine::Engines::Graphic
{
	template <typename Vertex, typename... Args>
	class IObjectGenerator
	{
	public:
		virtual std::tuple<std::vector<std::shared_ptr<Vertex>>, std::vector<std::shared_ptr<Scene::Face>>, Core::BoudingBox3D, glm::vec3> getObject(Args... args) = 0;

		std::shared_ptr<Scene::Mesh<Vertex>> getMesh(Args... args)
		{
			auto [vertices, faces, boudingBox, pivotPoint] = getObject(args...);
			auto mesh = std::make_shared<Scene::Mesh<Vertex>>(vertices, faces, boudingBox, pivotPoint);
			mesh->generate(static_cast<Common::VertexType>(Vertex::getType()));
			return mesh;
		}

		std::shared_ptr<Scene::Model<Vertex>> getModel(Args... args, const std::string& name = "")
		{
			std::vector<std::shared_ptr<Scene::Mesh<Vertex>>> meshes;
			auto mesh = getMesh(args...);
			meshes.push_back(mesh);
			return std::make_shared<Scene::Model<Vertex>>(meshes, mesh->getPivotPoint(), name);
		}

		std::shared_ptr<Scene::Face> buildFace(uint32_t i1, uint32_t i2, uint32_t i3, TriangleDirection direction)
		{
			return std::make_shared<Scene::Face>(direction == TriangleDirection::Clockwise ?
				Scene::Face({ i1, i2, i3 }) :
				Scene::Face({ i1, i3, i2 }));
		}
	};
}