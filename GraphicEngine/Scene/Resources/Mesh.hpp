#pragma once

#include "Face.hpp"
#include "../../Core/Math/Geometry.hpp"

#include <glm\geometric.hpp>

#include <functional>
#include <memory>
#include <utility>

namespace GraphicEngine::Scene
{
	template <typename Vertex>
	class Mesh
	{
	public:
		Mesh() {}
		Mesh(const std::vector<Vertex>& vertices, const std::vector<Face>& faces)
		{
			m_vertices = std::move(vertices);
			m_faces = std::move(faces);
		}

		template <typename VertexBufferFactory, typename VertexBuffer, typename... Args>
		std::shared_ptr<VertexBuffer> compile(Args&... args)
		{
			return VertexBufferFactory::produceVertexBuffer(args..., m_vertices, getIndices());
		}

		void addVertex(Vertex& vertex)
		{
			m_vertices.push_back(std::move(vertex));
		}

		void resizeVertices(uint32_t size)
		{
			m_vertices.reserve(size);
		}

		void addFace(Face& face)
		{
			m_faces.push_back(face);
		}

		void resizeFaces(uint32_t size)
		{
			m_faces.reserve(size);
		}

		std::vector<Face>& getFaces() const
		{
			return m_faces;
		}

		std::vector<Vertex>& getVertices() const 
		{
			return m_vertices;
		}

		std::vector<uint32_t> getIndices()
		{
			uint32_t totalIndices{ 0 };
			for (auto& face : m_faces)
			{
				totalIndices += face.indices.size();
			}

			std::vector<uint32_t> indices;
			for (auto& face : m_faces)
			{
				for (uint32_t index : face.indices)
				{
					indices.push_back(index);
				}
			}

			return std::move(indices);
		}

		void generate(Common::VertexType resources)
		{
			if (resources & Common::VertexType::Normal)
			{
				if constexpr (std::is_same_v<Vertex, Common::VertexPN> ||
					std::is_same_v<Vertex, Common::VertexPTcN> ||
					std::is_same_v<Vertex, Common::VertexPTcNTB>)
				{
					for (auto& face : m_faces)
					{
						std::vector<glm::vec3> vertices;
						vertices.reserve(face.indices.size());
						for (uint32_t index : face.indices)
						{
							vertices.push_back(m_vertices[index].position);
						}
						m_vertices[face.indices[0]].normal += Core::Math::calculateNormalFromPolygon(vertices);
					}

					for (auto& vertex : m_vertices)
					{
						vertex.normal = glm::normalize(vertex.normal);
					}
				}
			}

			if (resources & Common::VertexType::Tangent || resources & Common::VertexType::BiTangent)
			{
				if constexpr (std::is_same_v<Vertex, Common::VertexPTcNTB>)
				{
					for (auto& vertex : m_vertices)
					{
						glm::vec3 normal = vertex.normal;
						glm::vec3 c1 = glm::cross(normal, glm::vec3(0.0, 0.0, 1.0));
						glm::vec3 c2 = glm::cross(normal, glm::vec3(0.0, 1.0, 0.0));

						if (glm::length(c1) > glm::length(c2))
						{
							vertex.tangent = glm::normalize(c1);
						}
						
						else
						{
							vertex.tangent = glm::normalize(c2);
						}

						vertex.bitangent = glm::normalize(glm::cross(vertex.tangent, normal));
					}
				}
			}
		}
		
	private:
		std::vector<Vertex> m_vertices;
		std::vector<Face> m_faces;
	};
}
