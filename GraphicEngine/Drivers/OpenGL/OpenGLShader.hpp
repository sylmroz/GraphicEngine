#ifndef GRAPHIC_ENGINE_OPENGL_SHADER_HPP
#define GRAPHIC_ENGINE_OPENGL_SHADER_HPP

#include "../../Common/Shader.hpp"

#include <GL/glew.h>

namespace GraphicEngine::OpenGL
{
	class OpenGLShaderBase : public Shader
	{
	public:
		OpenGLShaderBase(const std::string& code, uint32_t shaderType) :
			Shader(code)
		{
			_shaderType = shaderType;
			compile();
		}

		template <typename Reader>
		OpenGLShaderBase(Reader reader,const std::string& path, uint32_t shaderType) :
			Shader(reader, path)
		{
			_shaderType = shaderType;
			compile();
		}

		uint32_t getShaderId()
		{
			return _shaderId;
		}

	protected:
		uint32_t _shaderType;
		uint32_t _shaderId;

		// Inherited via Shader
		virtual void compile() override;
	};

	class OpenGLVertexShader : public OpenGLShaderBase
	{
	public:
		OpenGLVertexShader(const std::string& code) :
			OpenGLShaderBase(code, GL_VERTEX_SHADER)
		{}

		template <typename Reader>
		OpenGLVertexShader(Reader reader, const std::string& path) :
			OpenGLShaderBase(reader, path, GL_VERTEX_SHADER) {}
	};

	class OpenGLFragmetShader : public OpenGLShaderBase
	{
	public:
		OpenGLFragmetShader(const std::string& code) :
			OpenGLShaderBase(code, GL_FRAGMENT_SHADER)
		{}

		template <typename Reader>
		OpenGLFragmetShader(Reader reader, const std::string& path) :
			OpenGLShaderBase(reader, path, GL_FRAGMENT_SHADER) {}
	};

	class OpenGLGeometryShader : public OpenGLShaderBase
	{
	public:
		OpenGLGeometryShader(const std::string& code) :
			OpenGLShaderBase(code, GL_GEOMETRY_SHADER)
		{}

		template <typename Reader>
		OpenGLGeometryShader(Reader reader, const std::string& path) :
			OpenGLShaderBase(reader, path, GL_GEOMETRY_SHADER) {}
	};

	class OpenGLTesselationControlShader : public OpenGLShaderBase
	{
	public:
		OpenGLTesselationControlShader(const std::string & code) :
			OpenGLShaderBase(code, GL_TESS_CONTROL_SHADER)
		{}

		template <typename Reader>
		OpenGLTesselationControlShader(Reader reader, const std::string& path) :
			OpenGLShaderBase(reader, path, GL_TESS_CONTROL_SHADER) {}
	};
}

#endif // !GRAPHIC_ENGINE_OPENGL_SHADER_HPP

