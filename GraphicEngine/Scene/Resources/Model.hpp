#pragma once

#include "Mesh.hpp"
#include <glm/gtx/transform.hpp>

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
			for (auto& m : m_meshes)
			{
				m_boudingBox.extendBox(m->getBoudingBox());
			}
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

		glm::vec3 getModelCenter()
		{
			return m_centralPosition;
		}

		void setPosition(glm::vec3 position)
		{
			m_position = position;
		}

		glm::vec3 getPosition()
		{
			return m_position;
		}

		void setScale(glm::vec3 scale)
		{
			m_scale = scale;
		}

		void setScale(float scale)
		{
			m_scale = glm::vec3(scale);
		}

		glm::vec3 getScale()
		{
			return m_scale;
		}

		void setRotate(glm::vec3 rotate)
		{
			m_rotation = rotate;
		}

		glm::vec3 getRotate()
		{
			return m_rotation;
		}

		glm::mat4 getModelMatrix()
		{
			glm::mat4 transform = glm::mat4(1.0f);
			transform = glm::translate(transform, m_position);

			transform = glm::rotate(transform, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
			transform = glm::rotate(transform, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
			transform = glm::rotate(transform, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
			
			transform = glm::scale(transform, m_scale);

			return transform;
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

	protected:
		glm::vec3 m_centralPosition; // average of vertex positions

		glm::vec3 m_position{ 0.0f,0.0f,0.0f };
		glm::vec3 m_rotation{ 0.0f,0.0f,0.0f };
		glm::vec3 m_scale{ 1.0f,1.0f,1.0f };

		Core::BoudingBox3D m_boudingBox;
	};
}