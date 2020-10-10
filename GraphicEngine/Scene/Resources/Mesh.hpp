#pragma once

#include "Face.hpp"
#include "Transformation.hpp"
#include "../../Core/Math/GeometryUtils.hpp"
#include "../../Core/Math/Geometry/3D/Octree.hpp"

#include <glm\geometric.hpp>

#include <functional>
#include <memory>
#include <utility>
#include <type_traits>

namespace GraphicEngine::Scene
{
	template <typename Vertex, int OctreeLevels = 5>
	class Mesh : public Transformation
	{
	public:
		Mesh() {}
		Mesh(const std::vector<std::shared_ptr<Vertex>>& vertices, const std::vector<std::shared_ptr<Face>>& faces)
		{
			m_vertices = std::move(vertices);
			m_faces = std::move(faces);
			generateBoudingBox();
		}

		Mesh(const std::vector<std::shared_ptr<Vertex>>& vertices, const std::vector<std::shared_ptr<Face>>& faces, Core::BoudingBox3D boudingBox)
		{
			m_vertices = std::move(vertices);
			m_faces = std::move(faces);
			m_boudingBox = boudingBox;
		}

		template <typename VertexBufferFactory, typename VertexBuffer, typename... Args>
		std::shared_ptr<VertexBuffer> compile(Args&... args)
		{
			return VertexBufferFactory::produceVertexBuffer(args..., getVertices(), getIndices());
		}

		void addVertex(std::shared_ptr<Vertex> vertex)
		{
			m_vertices.push_back(vertex);
			m_boudingBox.extendBox(vertex->position);
		}

		void resizeVertices(uint32_t size)
		{
			m_vertices.reserve(size);
		}

		void addFace(std::shared_ptr<Face> face)
		{
			m_faces.push_back(face);
		}

		void resizeFaces(uint32_t size)
		{
			m_faces.reserve(size);
		}

		std::vector<std::shared_ptr<Face>> getFaces()
		{
			return m_faces;
		}

		std::vector<Vertex> getVertices() 
		{
			std::vector<Vertex> vertices;
			vertices.reserve(m_vertices.size());
			for (auto vertex : m_vertices)
			{
				vertices.push_back(*vertex);
			}
			return vertices;
		}

		std::vector<std::shared_ptr<Vertex>> getVertices2()
		{
			return m_vertices;
		}

		std::vector<uint32_t> getIndices()
		{
			uint32_t totalIndices{ 0 };
			for (auto& face : m_faces)
			{
				totalIndices += face->indices.size();
			}

			std::vector<uint32_t> indices;
			indices.reserve(totalIndices);
			for (auto& face : m_faces)
			{
				for (uint32_t index : face->indices)
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
					std::vector<std::pair<uint32_t, glm::vec3>> normals;
					normals.resize(m_vertices.size());
					for (auto& face : m_faces)
					{
						std::vector<glm::vec3> vertices;
						vertices.reserve(face->indices.size());
						for (uint32_t index : face->indices)
						{
							vertices.push_back(m_vertices[index]->position);
						}

						for (uint32_t i{ 0 }; i < face->indices.size(); ++i)
						{
							auto& normal = normals[face->indices[i]];
							normal.first++;
							normal.second += Core::Math::calculateNormalFromPolygon(vertices);
						}
					}

					uint32_t i{ 0 };
					for (auto& vertex : m_vertices)
					{
						vertex->normal = glm::normalize(normals[i].second / static_cast<float>(normals[i].first));
						++i;
					}
				}
			}

			if (resources & Common::VertexType::Tangent || resources & Common::VertexType::BiTangent)
			{
				if constexpr (std::is_same_v<Vertex, Common::VertexPTcNTB>)
				{
					for (auto& vertex : m_vertices)
					{
						glm::vec3 normal = vertex->normal;
						glm::vec3 c1 = glm::cross(normal, glm::vec3(0.0, 0.0, 1.0));
						glm::vec3 c2 = glm::cross(normal, glm::vec3(0.0, 1.0, 0.0));

						if (glm::length(c1) > glm::length(c2))
						{
							vertex->tangent = glm::normalize(c1);
						}
						
						else
						{
							vertex->tangent = glm::normalize(c2);
						}

						vertex->bitangent = glm::normalize(glm::cross(vertex->tangent, normal));
					}
				}
			}

			m_octree = std::make_shared<Core::Octree<Vertex, OctreeLevels>>(m_boudingBox, m_vertices);
		}

		void generateBoudingBox()
		{
			for (auto v : m_vertices)
			{
				m_boudingBox.extendBox(v->position);
			}
		}

		Core::BoudingBox3D getBoudingBox()
		{
			return m_boudingBox;
		}

		std::shared_ptr<Core::Octree<Vertex>> getOctree()
		{
			return m_octree;
		}
		
	private:
		std::vector<std::shared_ptr<Vertex>> m_vertices;
		std::vector<std::shared_ptr<Face>> m_faces;

		std::shared_ptr<Core::Octree<Vertex, OctreeLevels>> m_octree;
	};
}
