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
				throw std::bad_typeid("Error when trying to find type: " + index.name());
			}
			m_entitiesLists[index].push_back(entity);
		}

		template <typename Func>
		void forEachEntity(Func func)
		{
			Core::Utils::for_each(VertexTypesRegister::types, [&](auto vertexType) 
			{
				for (const auto& entity : m_entitiesLists[decltype(vertexType)])
				{
					func(std::any_cast<std::shared_ptr<Entity<decltype(vertexType)>>(m_entitiesLists[decltype(vertexType)]);
				}
			});
		}
	protected:
		void initialize()
		{
			Core::Utils::for_each(VertexTypesRegister::types, [&](auto vertexType)
			{
				m_entitiesLists[decltype(vertexType)] = std::list<std::any>();
			});
		}
	protected:
		std::unordered_map<std::type_index, std::list<std::any>> m_entitiesLists;
	};
}