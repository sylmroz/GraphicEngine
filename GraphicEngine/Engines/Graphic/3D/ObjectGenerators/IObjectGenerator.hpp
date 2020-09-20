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

namespace GraphicEngine::Engines::Graphic::Generators
{
	template <typename Vertex, typename... Args>
	class IObjectGenerator
	{
	public:
		
		virtual std::tuple<std::vector<Vertex>, std::vector<Scene::Face>> getObject(Args... args) = 0;

		std::shared_ptr<Scene::Mesh<Vertex>> getMesh(Args... args)
		{
			auto [vertices, faces] = getObject(args...);
			auto mesh = std::make_shared<Scene::Mesh<Vertex>>(vertices, faces);
			mesh->generate(static_cast<Common::VertexType>(Vertex::getType()));
			return mesh;
		}

		std::shared_ptr<Scene::Model<Vertex>> getModel(Args... args, const std::string& name = "")
		{
			std::vector<std::shared_ptr<Scene::Mesh<Vertex>>> meshes;
			auto mesh = getMesh(args...);
			meshes.push_back(mesh);
			return std::make_shared<Scene::Model<Vertex>>(meshes, name);
		}
	};
}