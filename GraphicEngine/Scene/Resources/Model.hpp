#pragma once

#include "Mesh.hpp"

namespace GraphicEngine::Scene
{
	template <typename Vertex>
	class Model
	{
	public:
		Model() {}
		Model(std::vector<std::shared_ptr<Mesh<Vertex>>>& meshes, const std::string& name = "")
		{
			setMeshes(meshes, name);
		}

		void setMeshes(std::vector<std::shared_ptr<Mesh<Vertex>>>& meshes, const std::string& name = "")
		{
			m_meshes = std::move(meshes);
			m_name = name;
		}

		void setName(const std::string& name)
		{
			m_name = name;
		}

		std::string getName()
		{
			return m_name;
		}

		void addMesh(std::shared_ptr<Mesh<Vertex>> mesh)
		{
			m_meshes.push_back(mesh);
		}

		void reserveMeshes(uint32_t size)
		{
			m_meshes.reserve(size);
		}

		void setId(int32_t id)
		{
			m_id = id;
		}

		int32_t getId()
		{
			return m_id;
		}

		void addChildrenId(int32_t id)
		{
			m_childrens.push_back(id);
		}

		std::vector<int32_t> getChildrensId()
		{
			return m_childrens;
		}

		void setParentId(int32_t id)
		{
			m_parentId = id;
		}

		int32_t getParentId()
		{
			return m_parentId;
		}

		template <template<typename> typename VertexBufferFactory, template<typename> typename VertexBuffer, typename... Args>
		std::vector<std::shared_ptr<VertexBuffer<Vertex>>> compile(Args&... args)
		{
			std::vector<std::shared_ptr<VertexBuffer<Vertex>>> vertexBuffers;
			vertexBuffers.reserve(m_meshes.size());
			for (auto& mesh : m_meshes)
			{
				vertexBuffers.push_back(mesh->compile<VertexBufferFactory<Vertex>, VertexBuffer<Vertex>>(args...));
			}

			return vertexBuffers;
		}

	protected:
		std::vector<std::shared_ptr<Mesh<Vertex>>> m_meshes;
		std::string m_name;

		int32_t m_id{ -1 };
		std::vector<int32_t> m_childrens;
		int32_t m_parentId{ -1 };
	};
}