#pragma once
#include <vector>

namespace GraphicEngine::Scene
{
	struct Face
	{
		Face() {}
		Face(const std::vector<uint32_t>& indices) : indices{ indices } {}
		std::vector<uint32_t> indices;

		const std::vector<uint32_t>& getIndices()
		{
			return indices;
		}
	};
};