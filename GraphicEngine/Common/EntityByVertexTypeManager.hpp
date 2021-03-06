#pragma once

#include <any>
#include <list>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <execution>
#include <mutex>

#include "Vertex.hpp"
#include "../Core/Utils/UniqueIdentifier.hpp"

namespace GraphicEngine::Common
{
	template <template <typename> typename Entity>
	class EntityByVertexTypeManager
	{
	public:
		EntityByVertexTypeManager()
		{
			initialize();
		}

		template <typename VertexType>
		void addEntity(std::shared_ptr<Entity<VertexType>> entity)
		{
			auto index = std::type_index(typeid(VertexType));
			if (m_entitiesLists.find(index) == std::end(m_entitiesLists))
			{
				throw std::bad_typeid();
			}
			m_entitiesLists[index].push_back(entity);
		}

		template <typename VertexType>
		std::shared_ptr<Entity<VertexType>> getFirstEntity()
		{
			auto index = std::type_index(typeid(VertexType));
			if (m_entitiesLists.find(index) == std::end(m_entitiesLists))
			{
				throw std::bad_typeid();
			}
			return std::any_cast<std::shared_ptr<Entity<VertexType>>>(m_entitiesLists[index].front());
		}

		template <typename VertexType>
		void removeEntity(std::shared_ptr<Entity<VertexType>> entity)
		{
			auto index = std::type_index(typeid(VertexType));
			if (m_entitiesLists.find(index) == std::end(m_entitiesLists))
			{
				throw std::bad_typeid();
			}
			m_entitiesLists[index].remove(entity);
		}

		template <typename VertexType>
		void eraseEntity(std::list<std::any>::const_iterator entity)
		{
			auto index = std::type_index(typeid(VertexType));
			if (m_entitiesLists.find(index) == std::end(m_entitiesLists))
			{
				throw std::bad_typeid();
			}
			m_entitiesLists[index].erase(entity);
		}

		template <typename VertexType, typename Comparator>
		std::list<std::any>::const_iterator findIf(Comparator comparator)
		{
			auto index = std::type_index(typeid(VertexType));
			if (m_entitiesLists.find(index) == std::end(m_entitiesLists))
			{
				throw std::bad_typeid();
			}
			return std::find_if(std::begin(m_entitiesLists[index]), std::end(m_entitiesLists[index]), [&](std::any ent)
				{
					return comparator(std::any_cast<std::shared_ptr<Entity<VertexType>>>(ent));
				});
		}

		template <typename Func, typename ExecutionPolicy = decltype(std::execution::seq)>
		void forEachEntity(Func func, ExecutionPolicy policy = std::execution::seq)
		{
			Core::Utils::for_each(VertexTypesRegister::types, [&](auto vertexType)
				{
					auto index = std::type_index(typeid(decltype(vertexType)));
					/*for (auto entity : m_entitiesLists[index])
					{
						std::shared_ptr<Entity<decltype(vertexType)>> castEntity = std::any_cast<std::shared_ptr<Entity<decltype(vertexType)>>>(entity);
						func(castEntity);
					}*/
					std::mutex m;
					std::for_each(policy, std::begin(m_entitiesLists[index]), std::end(m_entitiesLists[index]), [&](auto entity) {
						std::lock_guard<std::mutex> guard(m);
						std::shared_ptr<Entity<decltype(vertexType)>> castEntity = std::any_cast<std::shared_ptr<Entity<decltype(vertexType)>>>(entity);
						func(castEntity);
						});
				});
		}
	protected:
		void initialize()
		{
			Core::Utils::for_each(VertexTypesRegister::types, [&](auto vertexType)
				{
					auto index = std::type_index(typeid(decltype(vertexType)));
					m_entitiesLists[index] = std::list<std::any>();
				});
		}
	protected:
		std::unordered_map<std::type_index, std::list<std::any>> m_entitiesLists;
	};
}