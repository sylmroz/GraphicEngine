#ifndef GRAPHIC_ENGINE_OPENGL_SHADER_HPP
#define GRAPHIC_ENGINE_OPENGL_SHADER_HPP

#include "../../Common/Shader.hpp"

#include <GL/glew.h>

#include <vector>

namespace GraphicEngine::OpenGL
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& code, const uint32_t shaderType) :
			Shader(code)
		{
			_shaderType = shaderType;
			OpenGLShader::compile();
		}

		template <typename Reader>
		OpenGLShader(Reader reader,const std::string& path, const uint32_t shaderType) :
			Shader(reader, path)
		{
			_shaderType = shaderType;
			OpenGLShader::compile();
		}

		~OpenGLShader()
		{
			glDeleteShader(_shaderId);
		}

		uint32_t getShaderId() const
		{
			return _shaderId;
		}

	protected:
		uint32_t _shaderType;
		uint32_t _shaderId;

		// Inherited via Shader
		virtual void compile() override;
	};

	class OpenGLVertexShader : public OpenGLShader
	{
	public:
		OpenGLVertexShader(const std::string& code) :
			OpenGLShader(code, GL_VERTEX_SHADER)
		{}

		template <typename Reader>
		OpenGLVertexShader(Reader reader, const std::string& path) :
			OpenGLShader(reader, path, GL_VERTEX_SHADER) {}
	};

	class OpenGLFragmentShader : public OpenGLShader
	{
	public:
		OpenGLFragmentShader(const std::string& code) :
			OpenGLShader(code, GL_FRAGMENT_SHADER)
		{}

		template <typename Reader>
		OpenGLFragmentShader(Reader reader, const std::string& path) :
			OpenGLShader(reader, path, GL_FRAGMENT_SHADER) {}
	};

	class OpenGLGeometryShader : public OpenGLShader
	{
	public:
		OpenGLGeometryShader(const std::string& code) :
			OpenGLShader(code, GL_GEOMETRY_SHADER)
		{}

		template <typename Reader>
		OpenGLGeometryShader(Reader reader, const std::string& path) :
			OpenGLShader(reader, path, GL_GEOMETRY_SHADER) {}
	};

	class OpenGLTesselationControlShader : public OpenGLShader
	{
	public:
		OpenGLTesselationControlShader(const std::string & code) :
			OpenGLShader(code, GL_TESS_CONTROL_SHADER)
		{}

		template <typename Reader>
		OpenGLTesselationControlShader(Reader reader, const std::string& path) :
			OpenGLShader(reader, path, GL_TESS_CONTROL_SHADER) {}
	};

	class OpenGLShaderProgram
	{
	public:
		OpenGLShaderProgram(const std::vector<OpenGLShader>& shaders)
		{
			_shaderProgramId = glCreateProgram();
			for (OpenGLShader shader : shaders)
			{
				glAttachShader(_shaderProgramId, shader.getShaderId());
			}
			glLinkProgram(_shaderProgramId);
		}

		uint32_t getShaderProgramId()
		{
			return _shaderProgramId;
		}

		void use()
		{
			glUseProgram(_shaderProgramId);
		}

		~OpenGLShaderProgram()
		{
			glDeleteProgram(_shaderProgramId);
		}
	protected:
		uint32_t _shaderProgramId;
	};
}

#endif // !GRAPHIC_ENGINE_OPENGL_SHADER_HPP

