#pragma once

#include <glm/mat4x2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "OpenGLShader.hpp"
#include "../../Core/Utils/GetClassName.hpp"

namespace GraphicEngine::OpenGL
{
	template <typename T>
	class ShaderStorageBufferObject
	{
	public:
		ShaderStorageBufferObject(const uint32_t index)
		{
			glGenBuffers(1, &m_ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * 126 + sizeof(uint32_t), nullptr, GL_STATIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		ShaderStorageBufferObject(const uint32_t index, std::shared_ptr<OpenGLShaderProgram> shaderProgram)
		{
			std::string blockName = Core::Utils::getClassName<T>();
			auto blockIndex = glGetProgramResourceIndex(shaderProgram->getShaderProgramId(), blockName.c_str());
			glShaderStorageBlockBinding(shaderProgram->getShaderProgramId(), blockIndex, index);
			glGenBuffers(1, &m_ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * 0, nullptr, GL_STATIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		void update(std::vector<T> values)
		{
			if (values.size() > 0)
			{
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
				auto size = values.size();
				glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(unsigned int), &size);
				glBufferSubData(GL_SHADER_STORAGE_BUFFER, 4 * sizeof(float), sizeof(T) * values.size(), &values[0]);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			}
		}

		void update(T& val, uint32_t index)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * index + 4 * sizeof(float), sizeof(T), &val);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

	protected:
		uint32_t m_ssbo;
		std::shared_ptr<OpenGLShaderProgram> m_shaderProgram;
	};
}
