#include "OpenGLShader.hpp"
#include <stdexcept>

void GraphicEngine::OpenGL::OpenGLShader::compile()
{
	_shaderId = glCreateShader(_shaderType);
	const char* shaderSource = reinterpret_cast<const char*>(m_data.data());
	const GLint shaderSourceLength = m_data.length();
	glShaderSource(_shaderId, 1, &shaderSource, &shaderSourceLength);
	glCompileShader(_shaderId);

	int succes;
	glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		std::string infoLog;
		infoLog.reserve(512);
		glGetShaderInfoLog(_shaderId, 512, nullptr, &infoLog[0]);
		std::string error = std::string("OpenGL shader compilation error! ");
		error += infoLog;
		throw std::runtime_error(infoLog);
	}
}

GraphicEngine::ShaderType GraphicEngine::OpenGL::GetShaderType(int shaderType)
{
	std::map<int, ShaderType> shaderTypeMap =
	{
		{ GL_VERTEX_SHADER, ShaderType::Vertex },
		{ GL_FRAGMENT_SHADER, ShaderType::Fragment },
		{ GL_GEOMETRY_SHADER, ShaderType::Geometry },
		{ GL_TESS_CONTROL_SHADER, ShaderType::TessalationControll },
		{ GL_TESS_EVALUATION_SHADER, ShaderType::TessalationEvaluation }
	};

	return shaderTypeMap[shaderType];
};
