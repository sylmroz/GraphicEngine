#pragma once

#include <glm/mat4x2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "OpenGLShader.hpp"

namespace GraphicEngine::OpenGL
{
	template <typename T>
	class UniformBuffer
	{
	public:
		UniformBuffer(const uint32_t index = 0)
		{
			glGenBuffers(1, &m_ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(T), nullptr, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferRange(GL_UNIFORM_BUFFER, index, m_ubo, 0, sizeof(T));
		}

		void update(T* val)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), val);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		template <typename Val>
		void update(Val* val, uint32_t stride)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, stride, sizeof(Val), val);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

	protected:
		uint32_t m_ubo;
	};
}
