#pragma once

#include <string>

namespace GraphicEngine
{
	enum class ShaderType
	{
		Vertex,
		Fragment,
		Geometry,
		TessalationControll,
		TessalationEvaluation
	};

	class Shader
	{
	public:
		Shader(ShaderType shaderType) :
			m_shaderType{ shaderType } {}

		Shader(const std::string& code, ShaderType shaderType) :
			m_shaderType{ shaderType }
		{
			m_data = code;
		}

		template <typename Reader, std::enable_if_t<std::is_function_v<Reader>, int> = 0>
		Shader(Reader reader, const std::string& path, ShaderType shaderType) :
			m_shaderType{ shaderType }
		{
			m_data = reader(path);
		}

		virtual void compile() = 0;
	protected:
		std::string m_data;
		ShaderType m_shaderType;
	};
}
