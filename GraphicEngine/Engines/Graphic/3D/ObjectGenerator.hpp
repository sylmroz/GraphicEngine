#pragma once

#include "ObjectGenerators/PlaneGenerator.hpp"

namespace GraphicEngine::Engines::Graphic
{
	enum class ObjectGeneratorType
	{
		Plane,
		Box,
		Cube,
		Pyramid,
		Cone,
		Cylinder
	};

	template <typename Vertex>
	class ObjectGenerator
	{
	public:
		template <typename... Args>
		static std::tuple<std::vector<Vertex>, std::vector<uin32_t>> getObject()
		{
		}
	};
}