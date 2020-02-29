#include "OpenGLShader.hpp"

void GraphicEngine::OpenGL::OpenGLShaderBase::compile()
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
		throw std::runtime_error("OpenGL shader compilation error!\h" + infoLog);
	}
}
