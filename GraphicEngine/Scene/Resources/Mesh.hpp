#pragma once

#include <functional>
#include <memory>

namespace GraphicEngine::Scene
{
	template <typename Vertex>
	class Mesh
	{
	public:
		Mesh() {}
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t> indices)
		{
			m_Vertices = std::move(vertices);
			m_Indices = std::move(indices);
		}

		template <typename VertexBufferFactory, typename VertexBuffer, typename... Args>
		std::shared_ptr<VertexBuffer> compile(Args&... args)
		{
			return VertexBufferFactory::produceVertexBuffer(args..., m_Vertices, m_Indices);
		}

		void addVertex(Vertex& vertex)
		{
			m_Vertices.push_back(std::move(vertex));
		}

		void resizeVertices(uint32_t size)
		{
			m_Vertices.resize(size);
		}

		void addIndex(uint32_t index)
		{
			m_Indices.push_back(index);
		}

		void resizeIndices(uint32_t size)
		{
			m_Indices.resize(size);
		}

		std::vector<Vertex>& getVertices() const 
		{
			return m_Vertices;
		}

		std::vector<uint32_t>& getIndices() const
		{
			return m_Indices;
		}

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}
