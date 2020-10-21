#include "pch.h"

#include "../GraphicEngine/Common/Vertex.hpp"
#include "../GraphicEngine/Engines/Graphic/3D/ObjectGenerators/CuboidGenerator.hpp"
#include "../GraphicEngine/Engines/Graphic/3D/ObjectGenerators/SphereGenerator.hpp"

using namespace GraphicEngine::Engines::Graphic;
using namespace GraphicEngine::Common;

TEST(Cuboid_Generator, Check_size_of_generated_vector)
{
	auto scale = glm::ivec3(2, 2, 3);
	auto verticesSize = 2 * (((scale.x + 1) * (scale.z + 1)) + ((scale.x + 1) * (scale.y - 1)) + ((scale.y - 1) * (scale.z - 1)));
	auto [vertices, faces, boudingBox] = CuboidGenerator<VertexPN>{}.getObject(glm::vec3(-10.0f), glm::vec3(10.0f), scale);
	EXPECT_EQ(vertices.size(), verticesSize);
}


TEST(Cuboid_Generator, Check_size_of_generated_vector_for_sphere)
{
	auto scale = glm::ivec2(6, 6);
	auto verticesSize = 2 + (scale.x * (scale.y - 1));
	auto [vertices, faces, boudingBox] = SphereGenerator<VertexPN>{}.getObject(glm::vec3(0.0f), 5 , scale);
	EXPECT_EQ(vertices.size(), verticesSize);
}