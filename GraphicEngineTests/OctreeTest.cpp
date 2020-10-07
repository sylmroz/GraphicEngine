#include "pch.h"

#include "../GraphicEngine/Core/Math/Geometry/3D/Octree.hpp"
#include "../GraphicEngine/Common/Vertex.hpp"

using namespace GraphicEngine::Core;
using namespace GraphicEngine::Common;

TEST(OctreeTest, Dynamic_CreateCorrect)
{
	BoudingBox3D boudingBox(glm::vec3(-10), glm::vec3(10));
	Octree<VertexP> p_octree(boudingBox);
	std::shared_ptr<VertexP> point = std::make_shared<VertexP>(glm::vec3(5));

	auto [node, level] = p_octree.findNode(point);
	EXPECT_EQ(node->aabb.getLeft(), boudingBox.getLeft());
}

TEST(OctreeTest, Static_CreateCorrect)
{
	BoudingBox3D boudingBox(glm::vec3(-10), glm::vec3(10));
	Octree<VertexP, 2> p_octree(boudingBox);
	std::shared_ptr<VertexP> point = std::make_shared<VertexP>(glm::vec3(-8));

	auto [node, level] = p_octree.findNode(point);
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


	auto [node, level] = p_octree.findNode(point);
	EXPECT_EQ(node->aabb.getLeft(), boudingBox.getLeft());
	EXPECT_EQ(node->aabb.getRight(), boudingBox.getCenter());

	auto [node2, level2] = p_octree.findNode(point2);
	EXPECT_EQ(node2->aabb.getRight(), boudingBox.getRight());
	EXPECT_EQ(node2->aabb.getLeft(), boudingBox.getCenter());
}

TEST(OctreeTest, Static_InsertPoint)
{
	BoudingBox3D boudingBox(glm::vec3(-10), glm::vec3(10));
	Octree<VertexP, 2, false> p_octree(boudingBox);
	std::shared_ptr<VertexP> point = std::make_shared<VertexP>(glm::vec3(-8));
	std::shared_ptr<VertexP> point2 = std::make_shared<VertexP>(glm::vec3(8));
	p_octree.insertPoint(point);
	p_octree.insertPoint(point2);

	auto [node, level] = p_octree.findNode(point);
	EXPECT_EQ(node->aabb.getLeft(), boudingBox.getLeft());
	EXPECT_EQ(node->aabb.getRight(), glm::vec3(-5));

	auto [node2, level2] = p_octree.findNode(point2);
	EXPECT_EQ(node2->aabb.getRight(), boudingBox.getRight());
	EXPECT_EQ(node2->aabb.getLeft(), glm::vec3(5));
}


TEST(OctreeTest, Static_InsertPoint_CheckAreOnTheList)
{
	BoudingBox3D boudingBox(glm::vec3(-10), glm::vec3(10));
	Octree<VertexP, 2, false> p_octree(boudingBox);
	std::shared_ptr<VertexP> point = std::make_shared<VertexP>(glm::vec3(-8));
	std::shared_ptr<VertexP> point2 = std::make_shared<VertexP>(glm::vec3(8));
	std::shared_ptr<VertexP> point3 = std::make_shared<VertexP>(glm::vec3(9));
	p_octree.insertPoint(point);
	p_octree.insertPoint(point2);
	p_octree.insertPoint(point3);

	auto [node, level] = p_octree.findNode(point2);
	std::list<std::shared_ptr<VertexP>> points = std::get<std::list<std::shared_ptr<VertexP>>>(node->element);
	std::vector<std::shared_ptr<VertexP>> expectedPoints{ point2, point3 };

	int i{ 0 };
	for (auto p : points)
	{
		EXPECT_EQ(p, expectedPoints[i]);
		++i;
	}
}

TEST(OctreeTest, Dynamic_InsertPointCheckNodeStoreCorrectPoint)
{
	BoudingBox3D boudingBox(glm::vec3(-10), glm::vec3(10));
	Octree<VertexP> p_octree(boudingBox);
	std::shared_ptr<VertexP> point = std::make_shared<VertexP>(glm::vec3(-8));
	std::shared_ptr<VertexP> point2 = std::make_shared<VertexP>(glm::vec3(8));
	std::shared_ptr<VertexP> point3 = std::make_shared<VertexP>(glm::vec3(9));
	p_octree.insertPoint(point);
	p_octree.insertPoint(point2);
	p_octree.insertPoint(point3);

	auto [node, level] = p_octree.findNode(std::make_shared<VertexP>(glm::vec3(-7)));
	EXPECT_EQ(std::get<std::shared_ptr<VertexP>>(node->element)->position, point->position);

	auto [node2, level2] = p_octree.findNode(std::make_shared<VertexP>(glm::vec3(8.4f)));
	EXPECT_EQ(std::get<std::shared_ptr<VertexP>>(node2->element)->position, point2->position);

	auto [node3, level3] = p_octree.findNode(std::make_shared<VertexP>(glm::vec3(9.4f)));
	EXPECT_EQ(std::get<std::shared_ptr<VertexP>>(node3->element)->position, point3->position);
}