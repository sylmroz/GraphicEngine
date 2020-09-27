#include "pch.h"

#include "../GraphicEngine/Core/Math/Geometry/3D/Octree.hpp"
#include "../GraphicEngine/Common/Vertex.hpp"

using namespace GraphicEngine::Engines::Graphic;
using namespace GraphicEngine::Common;

TEST(OctreeTest, Dynamic_CreateCorrect)
{
	BoudingBox3D boudingBox(glm::vec3(-10), glm::vec3(10));
	Octree<VertexP> p_octree(boudingBox);
	std::shared_ptr<VertexP> point = std::make_shared<VertexP>(glm::vec3(5));

	auto node = p_octree.findNode(point);
	EXPECT_EQ(node->aabb.getLeft(), boudingBox.getLeft());
}

TEST(OctreeTest, Static_CreateCorrect)
{
	BoudingBox3D boudingBox(glm::vec3(-10), glm::vec3(10));
	Octree<VertexP, 2> p_octree(boudingBox);
	std::shared_ptr<VertexP> point = std::make_shared<VertexP>(glm::vec3(-8));

	auto node = p_octree.findNode(point);
	EXPECT_EQ(node->aabb.getLeft(), boudingBox.getLeft());
}

TEST(OctreeTest, Dynamic_InsertPoint)
{
	BoudingBox3D boudingBox(glm::vec3(-10), glm::vec3(10));
	Octree<VertexP> p_octree(boudingBox);
	std::shared_ptr<VertexP> point = std::make_shared<VertexP>(glm::vec3(-8));
	std::shared_ptr<VertexP> point2 = std::make_shared<VertexP>(glm::vec3(8));
	p_octree.insertPoint(point);
	p_octree.insertPoint(point2);


	auto node = p_octree.findNode(point);
	EXPECT_EQ(node->aabb.getLeft(), boudingBox.getLeft());
	EXPECT_EQ(node->aabb.getRight(), boudingBox.getCenter());

	auto node2 = p_octree.findNode(point2);
	EXPECT_EQ(node2->aabb.getRight(), boudingBox.getRight());
	EXPECT_EQ(node2->aabb.getLeft(), boudingBox.getCenter());
}

TEST(OctreeTest, Static_InsertPoint)
{
	BoudingBox3D boudingBox(glm::vec3(-10), glm::vec3(10));
	Octree<VertexP, 2> p_octree(boudingBox);
	std::shared_ptr<VertexP> point = std::make_shared<VertexP>(glm::vec3(-8));
	std::shared_ptr<VertexP> point2 = std::make_shared<VertexP>(glm::vec3(8));
	p_octree.insertPoint(point);
	p_octree.insertPoint(point2);

	auto node = p_octree.findNode(point);
	EXPECT_EQ(node->aabb.getLeft(), boudingBox.getLeft());
	EXPECT_EQ(node->aabb.getRight(), glm::vec3(-5));

	auto node2 = p_octree.findNode(point2);
	EXPECT_EQ(node2->aabb.getRight(), boudingBox.getRight());
	EXPECT_EQ(node2->aabb.getLeft(), glm::vec3(5));
}