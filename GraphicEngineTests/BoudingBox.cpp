#include "pch.h"
#include "../GraphicEngine/Engines/Graphic/3D/BoudingBox3D.hpp"
#include "../GraphicEngine/Engines/Graphic/3D/BoudingBox3D.cpp"

using namespace GraphicEngine::Engines::Graphic;

TEST(BoudingBox, BoudaryCorrectnes)
{
	BoudingBox3D boudingBox(glm::vec3(0.0f), glm::vec3(1.0f));

	EXPECT_EQ(boudingBox.getLeft(), glm::vec3(0.0f));
}

TEST(BoudingBox, ExtendTest)
{
	BoudingBox3D boudingBox(glm::vec3(0.0f), glm::vec3(1.0f));
	boudingBox.extendBox(glm::vec3(5.5f, 0.0f, -1.0f));
	EXPECT_EQ(boudingBox.getLeft(), glm::vec3(0.0f, 0.0f, -1.0f));
	EXPECT_EQ(boudingBox.getRight(), glm::vec3(5.5f, 1.0f, 1.0f));
}

TEST(BoudingBox, BoudingBoxExtendTest)
{
	BoudingBox3D boudingBox(glm::vec3(0.0f), glm::vec3(1.0f));
	BoudingBox3D boudingBox2(glm::vec3(5.5f, 0.5f, -1.0f), glm::vec3(1.0f));

	boudingBox.extendBox(boudingBox2);
	EXPECT_EQ(boudingBox.getLeft(), glm::vec3(0.0f, 0.0f, -1.0f));
	EXPECT_EQ(boudingBox.getRight(), glm::vec3(5.5f, 1.0f, 1.0f));
}
