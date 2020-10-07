#pragma once

#include <array>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <variant>

#include "BoudingBox3D.hpp"

namespace GraphicEngine::Core
{
	enum Node3DOctan
	{
		PxPyPz, // x> y> z>
		NxPyPz, // x< y> z> 
		NxNyPz, // x< y< z>
		PxNyPz, // x> y< z>
		PxPyNz, // x> y> z<
		NxPyNz, // x< y> z<
		NxNyNz, // x< y< z<
		PxNyNz  // x> y< z<
	};

	template <typename T>
	struct Octan
	{
		std::variant<std::list<std::shared_ptr<T>>, std::shared_ptr<T>> element;
		std::shared_ptr<Octan<T>> parent;
		std::array<std::shared_ptr<Octan<T>>,8> childrens;
		BoudingBox3D aabb;
		bool hasValue{ false };
		bool hasChildrens{ false };
	};

	// If levels = 0 then we will split octan always will contain one elements
	// Otherwise octans can be splitted max Levels times and if octan cannot be splitted more then contain more than one element
	template <typename T, int Levels = 0, bool DynamicCreation = true>
	class Octree
	{
	public:
		Octree(BoudingBox3D aabb);
		Octree(BoudingBox3D aabb, std::vector<std::shared_ptr<T>> points);

		void insertPoint(std::shared_ptr<T> point);

		std::tuple<std::shared_ptr<Octan<T>>, int> findNode(std::shared_ptr<T> point);

	protected:
		std::shared_ptr<Octan<T>> createNode(BoudingBox3D aabb, std::shared_ptr<Octan<T>> parent);

		void generateChildrens(std::shared_ptr<Octan<T>> parent);

		BoudingBox3D generateAABBForNode(BoudingBox3D parentAABB, Node3DOctan part);

		void findNode(std::shared_ptr<T> point, std::shared_ptr<Octan<T>> currentNode, std::shared_ptr<Octan<T>>& retNode, int level, int& nodeLevel)
		{
			if (currentNode->hasChildrens && currentNode->aabb.isPointInside(point->position))
			{
				for (auto children : currentNode->childrens)
				{
					findNode(point, children, retNode, level + 1, nodeLevel);
				}
			}

			else if (!currentNode->hasChildrens && currentNode->aabb.isPointInside(point->position))
			{
				retNode = currentNode;
				nodeLevel = level;
			}
		}

		BoudingBox3D generateBoudingBoxPxPyPz(glm::vec3 left, glm::vec3 center, glm::vec3 right)
		{
			return BoudingBox3D(center, right);
		}

		BoudingBox3D generateBoudingBoxNxPyPz(glm::vec3 left, glm::vec3 center, glm::vec3 right)
		{
			return BoudingBox3D(glm::vec3(left.x, center.y, center.z), glm::vec3(center.x, right.y, right.z));
		}

		BoudingBox3D generateBoudingBoxNxNyPz(glm::vec3 left, glm::vec3 center, glm::vec3 right)
		{
			return BoudingBox3D(glm::vec3(left.x, left.y, center.z), glm::vec3(center.x, center.y, right.z));
		}

		BoudingBox3D generateBoudingBoxPxNyPz(glm::vec3 left, glm::vec3 center, glm::vec3 right)
		{
			return BoudingBox3D(glm::vec3(center.x, left.y, center.z), glm::vec3(right.x, center.y, right.z));
		}

		BoudingBox3D generateBoudingBoxPxPyNz(glm::vec3 left, glm::vec3 center, glm::vec3 right)
		{
			return BoudingBox3D(glm::vec3(center.x, center.y, left.z), glm::vec3(right.x, right.y, center.z));
		}

		BoudingBox3D generateBoudingBoxNxPyNz(glm::vec3 left, glm::vec3 center, glm::vec3 right)
		{
			return BoudingBox3D(glm::vec3(left.x, center.y, left.z), glm::vec3(center.x, right.y, center.z));
		}

		BoudingBox3D generateBoudingBoxNxNyNz(glm::vec3 left, glm::vec3 center, glm::vec3 right)
		{
			return BoudingBox3D(left, center);
		}

		BoudingBox3D generateBoudingBoxPxNyNz(glm::vec3 left, glm::vec3 center, glm::vec3 right)
		{
			return BoudingBox3D(glm::vec3(center.x, left.y, left.z), glm::vec3(right.x, center.y, center.z));
		}

		void buidChidrenBoudingBoxGenerator();

		void buildChidrensRecusivelly(std::shared_ptr<Octan<T>> parent, int level);

		std::map<int, Node3DOctan> getIntToEnumMapping();

	protected:
		std::shared_ptr<Octan<T>> m_root;
		std::map<Node3DOctan, std::function<BoudingBox3D(glm::vec3, glm::vec3, glm::vec3)>> m_boudingBoxGenerators;
	};

	template<typename T, int Levels, bool DynamicCreation>
	inline Octree<T, Levels, DynamicCreation>::Octree(BoudingBox3D aabb)
	{
		m_root = createNode(aabb, nullptr);
		buidChidrenBoudingBoxGenerator();
		if (Levels > 0 && !DynamicCreation)
		{
			buildChidrensRecusivelly(m_root, 0);
		}
	}

	template<typename T, int Levels, bool DynamicCreation>
	inline Octree<T, Levels, DynamicCreation>::Octree(BoudingBox3D aabb, std::vector<std::shared_ptr<T>> points):
		Octree<T,Levels>(aabb)
	{
		for (auto p : points)
		{
			insertPoint(p);
		}
	}
	
	template<typename T, int Levels, bool DynamicCreation>
	inline std::shared_ptr<Octan<T>> Octree<T, Levels, DynamicCreation>::createNode(BoudingBox3D aabb, std::shared_ptr<Octan<T>> parent)
	{
		std::shared_ptr<Octan<T>> octan = std::make_shared<Octan<T>>();
		octan->aabb = aabb;
		octan->parent = parent;
		if constexpr (Levels == 0)
		{
			octan->element = std::make_shared<T>();
		}

		else
		{
			octan->element = std::list<std::shared_ptr<T>>();
		}
		return octan;
	}
	
	template<typename T, int Levels, bool DynamicCreation>
	inline void Octree<T, Levels, DynamicCreation>::generateChildrens(std::shared_ptr<Octan<T>> parent)
	{
		auto mapping = getIntToEnumMapping();
		for (uint32_t i{ 0 }; i < 8; ++i)
		{
			auto part = mapping[i];
			parent->childrens[part] = createNode(generateAABBForNode(parent->aabb, part), parent);
		}
		parent->hasChildrens = true;
	}
	
	template<typename T, int Levels, bool DynamicCreation>
	inline BoudingBox3D Octree<T, Levels, DynamicCreation>::generateAABBForNode(BoudingBox3D parentAABB, Node3DOctan part)
	{
		return m_boudingBoxGenerators[part](parentAABB.getLeft(), parentAABB.getCenter(), parentAABB.getRight());
	}
	
	template<typename T, int Levels, bool DynamicCreation>
	inline void Octree<T, Levels, DynamicCreation>::insertPoint(std::shared_ptr<T> point)
	{
		auto [node, level] = findNode(point);
		if constexpr (Levels == 0)
		{
			if (node->hasValue)
			{
				std::shared_ptr<T> value = std::get<std::shared_ptr<T>>(node->element);
				generateChildrens(node);
				insertPoint(point);
				insertPoint(value);
			}

			else
			{
				node->element = point;
				node->hasValue = true;
			}
		}

		else if constexpr (DynamicCreation)
		{
			std::list<std::shared_ptr<T>>& points = std::get<std::list<std::shared_ptr<T>>>(node->element);
			if (level <= Levels)
			{
				if (points.size() == 0 || level == Levels)
				{
					points.push_back(point);
					return;
				}
				generateChildrens(node);
				for (auto p : points)
				{
					insertPoint(p);
				}
				insertPoint(point);
				points.clear();
			}

			else
			{
				points.push_back(point);
			}
		}

		else
		{
			std::get<std::list<std::shared_ptr<T>>>(node->element).push_back(point);
		}
	}

	template<typename T, int Levels, bool DynamicCreation>
	inline std::tuple<std::shared_ptr<Octan<T>>, int> Octree<T, Levels, DynamicCreation>::findNode(std::shared_ptr<T> point)
	{
		int level{ 0 };
		std::shared_ptr<Octan<T>> node;
		findNode(point, m_root, node, 0, level);
		return std::make_tuple(node, level);
	}

	template<typename T, int Levels, bool DynamicCreation>
	inline void Octree<T, Levels, DynamicCreation>::buidChidrenBoudingBoxGenerator()
	{
		m_boudingBoxGenerators = std::map<Node3DOctan, std::function<BoudingBox3D(glm::vec3, glm::vec3, glm::vec3)>>{
			{ PxPyPz, [&](glm::vec3 left, glm::vec3 center, glm::vec3 right) { return generateBoudingBoxPxPyPz(left,center, right);}},
			{ NxPyPz, [&](glm::vec3 left, glm::vec3 center, glm::vec3 right) { return generateBoudingBoxNxPyPz(left,center, right); }},
			{ NxNyPz, [&](glm::vec3 left, glm::vec3 center, glm::vec3 right) { return generateBoudingBoxNxNyPz(left,center, right); } },
			{ PxNyPz, [&](glm::vec3 left, glm::vec3 center, glm::vec3 right) { return generateBoudingBoxPxNyPz(left,center, right);}},
			{ PxPyNz, [&](glm::vec3 left, glm::vec3 center, glm::vec3 right) { return generateBoudingBoxPxPyNz(left,center, right); }},
			{ NxPyNz, [&](glm::vec3 left, glm::vec3 center, glm::vec3 right) { return generateBoudingBoxNxPyNz(left,center, right); }},
			{ NxNyNz, [&](glm::vec3 left, glm::vec3 center, glm::vec3 right) { return generateBoudingBoxNxNyNz(left,center, right); }},
			{ PxNyNz, [&](glm::vec3 left, glm::vec3 center, glm::vec3 right) { return generateBoudingBoxPxNyNz(left,center, right); }},
		};
	}
	template<typename T, int Levels, bool DynamicCreation>
	inline void Octree<T, Levels, DynamicCreation>::buildChidrensRecusivelly(std::shared_ptr<Octan<T>> parent, int level)
	{
		int endLevel = Levels;
		if (level == endLevel)
			return;
		generateChildrens(parent);
		for (auto children : parent->childrens)
		{
			buildChidrensRecusivelly(children, level + 1);
		}
	}
	template<typename T, int Levels, bool DynamicCreation>
	inline std::map<int, Node3DOctan> Octree<T, Levels, DynamicCreation>::getIntToEnumMapping()
	{
		std::map<int, Node3DOctan> mapping
		{
			{ 0, PxPyPz},
			{ 1, NxPyPz},
			{ 2, NxNyPz},
			{ 3, PxNyPz},
			{ 4, PxPyNz},
			{ 5, NxPyNz},
			{ 6, NxNyNz},
			{ 7, PxNyNz}
		};
		return mapping;
	}
}