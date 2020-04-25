#pragma once

#include <glm/mat4x2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "OpenGLShader.hpp"

namespace GraphicEngine::OpenGL
{
	template <typename Uniform, typename T>
	class UniformData
	{
	public:
		UniformData(std::shared_ptr<OpenGLShaderProgram> openGlShaderProgram, const std::string& name) :
			m_openGlShaderProgram(openGlShaderProgram),
			m_name(name)
		{}

		void update(T val)
		{
			static_cast<Uniform const&>(*this).update(val);
		}
		
	protected:
		std::shared_ptr<OpenGLShaderProgram> m_openGlShaderProgram;
		std::string m_name;
	};


	class UniformBufferMatrix : public UniformData<UniformBufferMatrix, glm::mat4>
	{
	public:
		UniformBufferMatrix(std::shared_ptr<OpenGLShaderProgram> openGlShaderProgram, const std::string& name) :
			UniformData<UniformBufferMatrix, glm::mat4>(openGlShaderProgram, name)
		{}

		void update(const glm::mat4& val)
		{
			auto uniformLocation = glGetUniformLocation(m_openGlShaderProgram->getShaderProgramId(), m_name.c_str());
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(val));
		}
	};

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
