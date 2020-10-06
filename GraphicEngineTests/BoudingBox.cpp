#include "pch.h"
#include "../GraphicEngine/Core/Math/Geometry/3D/BoudingBox3D.hpp"
#include "../GraphicEngine/Core/Math/Geometry/3D/BoudingBox3D.cpp"
#include <glm/gtx/transform.hpp>

using namespace GraphicEngine::Core;

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

TEST(BoudingBox, BoudingBoxScaleTransform)
{
	BoudingBox3D boudingBox(glm::vec3(1.0f), glm::vec3(-1.0f));
	boudingBox.transform(glm::scale(glm::vec3(2.0f)));

	EXPECT_EQ(boudingBox.getLeft(), glm::vec3(-2.0f));
	EXPECT_EQ(boudingBox.getRight(), glm::vec3(2.0f));

	boudingBox.transform(glm::scale(glm::vec3(1.5f)));

	EXPECT_EQ(boudingBox.getLeft(), glm::vec3(-1.5f));
	EXPECT_EQ(boudingBox.getRight(), glm::vec3(1.5f));
}

TEST(BoudingBox, BoudingBoxTranslateTransform)
{
	BoudingBox3D boudingBox(glm::vec3(0.0f), glm::vec3(1.0f));
	boudingBox.transform(glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));

	EXPECT_EQ(boudingBox.getLeft(), glm::vec3(0.0f, 1.0f, 0.0f));
	EXPECT_EQ(boudingBox.getRight(), glm::vec3(1.0f, 2.0f, 1.0f));

	boudingBox.transform(glm::translate(glm::vec3(0.0f)));

	EXPECT_EQ(boudingBox.getLeft(), glm::vec3(0.0f));
	EXPECT_EQ(boudingBox.getRight(), glm::vec3(1.0f));
}