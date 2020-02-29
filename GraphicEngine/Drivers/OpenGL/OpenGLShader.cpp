#include "OpenGLShader.hpp"

void GraphicEngine::OpenGL::OpenGLShader::compile()
{
	_shaderId = glCreateShader(_shaderType);
	const char* shaderSource = reinterpret_cast<const char*>(_data.data());
	const GLint shaderSourceLenmgth = _data.length();
	glShaderSource(_shaderId, 1, &shaderSource, &shaderSourceLenmgth);
	glCompileShader(_shaderId);

	int succes;
	glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		std::string infoLog;
		infoLog.reserve(512);
		glGetShaderInfoLog(_shaderId, 512, nullptr, &infoLog[0]);
		std::string error = std::string("OpenGL shader compilation error! ");
		error+=infoLog;
		throw std::runtime_error(infoLog);
	}
}
