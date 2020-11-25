#pragma once

#include <any>
#include <list>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Vertex.hpp"

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
				//std::string("Error when trying to find type: ") + std::string(index.name()).c_str();
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
				//std::string("Error when trying to find type: ") + std::string(index.name()).c_str();
				throw std::bad_typeid();
			}
			return  std::any_cast<std::shared_ptr<Entity<VertexType>>>(m_entitiesLists[index].front());
		}

		template <typename Func>
		void forEachEntity(Func func)
		{
			Core::Utils::for_each(VertexTypesRegister::types, [&](auto vertexType) 
			{
				auto index = std::type_index(typeid(decltype(vertexType)));
				for (const auto& entity : m_entitiesLists[index])
				{
					for (auto entity : m_entitiesLists[index])
					{
						std::shared_ptr<Entity<decltype(vertexType)>> castEntity = std::any_cast<std::shared_ptr<Entity<decltype(vertexType)>>>(entity);
						func(castEntity);
					}
				}
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