#pragma once

#include <GL/glew.h>

#include <memory>
#include <vector>

namespace GraphicEngine::OpenGL
{
	template <typename _Vertex>
	class VertexBuffer
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

			void bind() const
			{
				glBindVertexArray(_vao);
			}

			virtual void draw()
			{
				glDrawArrays(GL_TRIANGLES, 0, _size);
			}

			void unbind() const
			{
				glBindVertexArray(0);
			}

			virtual ~_VertexBuffer() = default;

		protected:
			GLuint _vbo, _vao;
			GLsizei _size;
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
			virtual void draw() override
			{
				glDrawElements(GL_TRIANGLES, this->_size, GL_UNSIGNED_INT, nullptr);
			}

			virtual ~_VertexBufferWithElements() = default;
		protected:
			GLuint _ebo;
		};

	public:
		VertexBuffer(const std::vector<_Vertex>& vertices)
		{
			m_data = std::unique_ptr<_VertexBuffer>(new _VertexBuffer(vertices));
		}
		VertexBuffer(const std::vector<_Vertex>& vertices, const std::vector<uint32_t>& indices)
		{
			m_data = std::unique_ptr<_VertexBuffer>(new _VertexBufferWithElements(vertices, indices));
		}

		void bind() const
		{
			m_data->bind();
		}

		virtual void draw()
		{
			m_data->draw();
		}

		void unbind() const
		{
			m_data->unbind();
		}
	private:
		std::unique_ptr<_VertexBuffer> m_data;
	};
}
