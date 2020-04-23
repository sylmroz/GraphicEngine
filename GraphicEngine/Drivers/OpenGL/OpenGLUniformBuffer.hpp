#pragma once

#include <glm/mat4x2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "OpenGLShader.hpp"

namespace GraphicEngine::OpenGL
{
	template <typename Uniform, typename T>
	class UniformBuffer
	{
	public:
		UniformBuffer(std::shared_ptr<OpenGLShaderProgram> openGlShaderProgram, const std::string& name) :
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


	class UniformBufferMatrix : public UniformBuffer<UniformBufferMatrix, glm::mat4>
	{
	public:
		UniformBufferMatrix(std::shared_ptr<OpenGLShaderProgram> openGlShaderProgram, const std::string& name) :
			UniformBuffer<UniformBufferMatrix, glm::mat4>(openGlShaderProgram, name)
		{}

		void update(const glm::mat4& val)
		{
			auto uniformLocation = glGetUniformLocation(m_openGlShaderProgram->getShaderProgramId(), m_name.c_str());
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(val));
		}
	};
}
