#include "pch.h"

#include "../GraphicEngine/Common/Vertex.hpp"
#include "../GraphicEngine/Engines/Graphic/3D/ObjectGenerators/CuboidGenerator.hpp"

using namespace GraphicEngine::Engines::Graphic;
using namespace GraphicEngine::Common;

TEST(Cuboid_Generator, Check_size_of_generated_vector)
{
	auto scale = glm::ivec3(5, 20, 30);
	auto verticesSize = 2 * (((scale.x + 1) * (scale.z + 1)) + ((scale.x + 1) * (scale.y - 1)) + ((scale.y - 1) * (scale.z - 1)));
	auto [vertices, faces, boudingBox] = CuboidGenerator<VertexPN>{}.getObject(glm::vec3(-10.0f), glm::vec3(10.0f), scale);
	EXPECT_EQ(vertices.size(), verticesSize);
}