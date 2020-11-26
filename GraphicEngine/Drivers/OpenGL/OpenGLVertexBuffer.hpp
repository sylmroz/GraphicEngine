#pragma once

#include "../../Common/VertexBuffer.hpp"

#include <GL/glew.h>

#include <memory>
#include <stdexcept>
#include <vector>

namespace GraphicEngine::OpenGL
{
	template <typename _Vertex>
	class VertexBuffer : public Common::VertexBuffer<OpenGL::VertexBuffer<_Vertex>, int>
	{
		class _IVerexBuffer
		{
		public:
			virtual void bind(int dummy = 0) const = 0;

			virtual void bindSecond(int dummy = 0) const = 0;

			virtual void draw(int primitiveTopology) = 0;

			virtual void drawElements(int primitiveTopology) = 0;

			virtual void drawEdges(int primitiveTopology) = 0;

			virtual void unbind(int dummy = 0) const = 0;

			virtual ~_IVerexBuffer() = default;
		};


		class _VertexBuffer : public _IVerexBuffer
		{
		public:
			_VertexBuffer() {}
			_VertexBuffer(const std::vector<_Vertex>& vertices)
			{
				this->m_vertexBufferSize = vertices.size();
				glGenVertexArrays(1, &m_vao);
				glGenBuffers(1, &m_vbo);

				bind();
				glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

				glBufferData(GL_ARRAY_BUFFER, vertices.size() * _Vertex::getStride(), vertices.data(), GL_STATIC_DRAW);

				std::vector<std::pair<uint32_t, uint32_t>> sizesAndOffsets = _Vertex::getSizeAndOffsets();

				uint32_t i{ 0 };
				for (auto sizeAndOffset : sizesAndOffsets)
				{
					glEnableVertexAttribArray(i);
					glVertexAttribPointer(i, sizeAndOffset.first, GL_FLOAT, GL_FALSE, _Vertex::getStride(), reinterpret_cast<void*>(sizeAndOffset.second));
					++i;
				}
				unbind();
			}

			virtual void bind(int dummy = 0) const override
			{
				glBindVertexArray(m_vao);
			}

			virtual void bindSecond(int dummy = 0) const override
			{
				throw std::logic_error("Function not yet implemented");
			}

			virtual void draw(int primitiveTopology)
			{
				glDrawArrays(primitiveTopology, 0, m_vertexBufferSize);
			}

			virtual void drawElements(int primitiveTopology)
			{
				throw std::logic_error("Function not yet implemented");
			}

			virtual void drawEdges(int primitiveTopology)
			{
				throw std::logic_error("Function not yet implemented");
			}

			virtual void unbind(int dummy = 0) const override
			{
				glBindVertexArray(0);
			}

			virtual ~_VertexBuffer() = default;

		protected:
			GLuint m_vbo{ 0 }, m_vao{ 0 };
			GLsizei m_vertexBufferSize{ 0 };
		};

		class _VertexBufferWithElements : public _VertexBuffer
		{
		public:
			_VertexBufferWithElements() {}
			_VertexBufferWithElements(const std::vector<_Vertex>& vertices, const std::vector<uint32_t>& indices)
			{
				this->m_indicesBufferSize = indices.size();
				this->m_vertexBufferSize = vertices.size();
				glGenVertexArrays(1, &this->m_vao);
				glGenBuffers(1, &this->m_vbo);
				glGenBuffers(1, &this->m_ebo);

				this->bind();
				glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * _Vertex::getStride(), vertices.data(), GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

				std::vector<std::pair<uint32_t, uint32_t>> sizesAndOffsets = _Vertex::getSizeAndOffsets();

				uint32_t i{ 0 };
				for (auto sizeAndOffset : sizesAndOffsets)
				{
					glEnableVertexAttribArray(i);
					glVertexAttribPointer(i, sizeAndOffset.first, GL_FLOAT, GL_FALSE, _Vertex::getStride(), reinterpret_cast<void*>(sizeAndOffset.second));
					++i;
				}
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				this->unbind();
			}

			virtual void drawElements(int primitiveTopology) override
			{
				glDrawElements(primitiveTopology, this->m_indicesBufferSize, GL_UNSIGNED_INT, nullptr);
			}

			virtual void draw(int primitiveTopology) override
			{
				glDrawArrays(primitiveTopology, 0, this->m_vertexBufferSize);
			}

			virtual ~_VertexBufferWithElements() = default;
		protected:
			GLuint m_ebo;
			uint32_t m_indicesBufferSize;
		};

		class _VertexBufferWithElementsAndEdges : public _IVerexBuffer
		{
		public:
			_VertexBufferWithElementsAndEdges() = default;
			_VertexBufferWithElementsAndEdges(const std::vector<_Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& edges)
			{
				m_elements = std::make_unique<_VertexBufferWithElements>(vertices, indices);
				m_edges = std::make_unique<_VertexBufferWithElements>(vertices, edges);
			}
			virtual void bind(int dummy = 0) const override
			{
				m_elements->bind();
			}

			virtual void bindSecond(int dummy = 0) const override
			{
				m_edges->bind();
			}

			virtual void draw(int primitiveTopology) override
			{
				m_elements->draw(primitiveTopology);
			}

			virtual void drawElements(int primitiveTopology) override
			{
				m_elements->bind();
				m_elements->drawElements(primitiveTopology);
			}

			virtual void drawEdges(int primitiveTopology) override
			{
				m_edges->bind();
				m_edges->drawElements(primitiveTopology);
			}

			virtual void unbind(int dummy = 0) const override
			{
				m_elements->unbind();
			}

			virtual ~_VertexBufferWithElementsAndEdges() = default;

		private:
			std::unique_ptr<_VertexBufferWithElements> m_elements;
			std::unique_ptr<_VertexBufferWithElements> m_edges;
		};

	public:
		using VertexType = _Vertex;

		VertexBuffer(const std::vector<VertexType>& vertices)
		{
			m_data = std::make_unique<_VertexBuffer>(vertices);
		}
		VertexBuffer(const std::vector<_Vertex>& vertices, const std::vector<uint32_t>& indices)
		{
			m_data = std::make_unique<_VertexBufferWithElements>(vertices, indices);
		}

		VertexBuffer(const std::vector<_Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& edges)
		{
			m_data = std::make_unique<_VertexBufferWithElementsAndEdges>(vertices, indices, edges);
		}

		void bind(int dummy = 0) const
		{
			m_data->bind();
		}

		void draw(int primitiveTopology)
		{
			m_data->draw(primitiveTopology);
		}

		void drawElements(int primitiveTopology)
		{
			m_data->drawElements(primitiveTopology);
		}

		void drawEdges(int primitiveTopology)
		{
			m_data->drawEdges(primitiveTopology);
		}

		void unbind(int dummy = 0) const
		{
			m_data->unbind();
		}
	private:
		std::unique_ptr<_IVerexBuffer> m_data;
	};
}
