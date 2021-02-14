#pragma once

#include <glm/mat4x2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "OpenGLShader.hpp"
#include "../../Core/Utils/GetClassName.hpp"

namespace GraphicEngine::OpenGL
{
	class UniformBufferBase
	{
	public:
		UniformBufferBase(const uint32_t index, uint32_t size)
		{
			glGenBuffers(1, &m_ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferRange(GL_UNIFORM_BUFFER, index, m_ubo, 0, size);
		}

		UniformBufferBase(const uint32_t index, std::shared_ptr<OpenGLShaderProgram> shaderProgram, std::string blockName, uint32_t size)
		{
			auto blockIndex = glGetUniformBlockIndex(shaderProgram->getShaderProgramId(), blockName.c_str());
			glUniformBlockBinding(shaderProgram->getShaderProgramId(), blockIndex, index);
			glGenBuffers(1, &m_ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferBase(GL_UNIFORM_BUFFER, index, m_ubo);
		}

		template <typename Val>
		void update(Val* val, uint32_t count, uint32_t stride)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, stride, sizeof(Val) * count, val);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

	protected:
		uint32_t m_ubo;
	};

	template <typename T>
	class UniformBuffer : public UniformBufferBase
	{
	public:
		UniformBuffer(const uint32_t index) :
			UniformBufferBase(index, sizeof(T))
		{}

		UniformBuffer(const uint32_t index, std::shared_ptr<OpenGLShaderProgram> shaderProgram) :
			UniformBufferBase(index, shaderProgram, Core::Utils::getClassName<T>(), sizeof(T))
		{}

		void update(T* val)
		{
			UniformBufferBase::update(val, 1, 0);
		}

	protected:
		uint32_t m_ubo;
	};

	template <typename T, int N = 128>
	class UniformBufferArray : public UniformBufferBase
	{
	public:
		UniformBufferArray(uint32_t index) :
			UniformBufferBase(index, sizeof(T::type_base) * N)
		{}

		UniformBufferArray(const uint32_t index, std::shared_ptr<OpenGLShaderProgram> shaderProgram) :
			UniformBufferBase(index, shaderProgram, Core::Utils::getClassName<T>(), sizeof(T::type_base) * N)
		{}

		/*void updateArr(T val)
		{
			UniformBufferBase::update(val.data.data(), data.size(), 0);
		}*/
	};
}
