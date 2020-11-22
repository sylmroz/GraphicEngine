#pragma once

#include "../../Common/VertexBuffer.hpp"

#include <GL/glew.h>

#include <memory>
#include <vector>

namespace GraphicEngine::OpenGL
{
	template <typename _Vertex>
	class VertexBuffer : public Common::VertexBuffer<OpenGL::VertexBuffer<_Vertex>, int>
	{
		class _VertexBuffer
		{
		public:
			_VertexBuffer() {}
			_VertexBuffer(const std::vector<_Vertex>& vertices)
			{
				this->_size = vertices.size();
				glGenVertexArrays(1, &_vao);
				glGenBuffers(1, &_vbo);

				bind();
				glBindBuffer(GL_ARRAY_BUFFER, _vbo);

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

			void bind(int bummy = 0) const
			{
				glBindVertexArray(_vao);
			}

			virtual void draw(int primitiveTopology)
			{
				glDrawArrays(primitiveTopology, 0, _size);
			}

			void unbind(int dummy = 0) const
			{
				glBindVertexArray(0);
			}

			virtual ~_VertexBuffer() = default;

		protected:
			GLuint _vbo{ 0 }, _vao{ 0 };
			GLsizei _size{ 0 };
		};

		class _VertexBufferWithElements : public _VertexBuffer
		{
		public:
			_VertexBufferWithElements() {}
			_VertexBufferWithElements(const std::vector<_Vertex>& vertices, const std::vector<uint32_t>& indices)
			{
				this->_size = indices.size();
				glGenVertexArrays(1, &this->_vao);
				glGenBuffers(1, &this->_vbo);
				glGenBuffers(1, &this->_ebo);

				this->bind();
				glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * _Vertex::getStride(), vertices.data(), GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
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
			virtual void draw(int primitiveTopology) override
			{
				glDrawElements(primitiveTopology, this->_size, GL_UNSIGNED_INT, nullptr);
			}

			virtual ~_VertexBufferWithElements() = default;
		protected:
			GLuint _ebo;
		};

	public:
		using VertexType = _Vertex;

		VertexBuffer(const std::vector<VertexType>& vertices)
		{
			m_data = std::make_unique<_VertexBuffer>(vertices);
		}
		VertexBuffer(const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices)
		{
			m_data = std::make_unique<_VertexBufferWithElements>(vertices, indices);
		}

		void bind(int dummy = 0) const
		{
			m_data->bind();
		}

		void draw(int primitiveTopology)
		{
			m_data->draw(primitiveTopology);
		}

		void unbind(int dummy = 0) const
		{
			m_data->unbind();
		}
	private:
		std::unique_ptr<_VertexBuffer> m_data;
	};
}
