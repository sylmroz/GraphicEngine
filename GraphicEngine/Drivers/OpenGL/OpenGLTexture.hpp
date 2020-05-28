#pragma once

#include <array>
#include <string>
#include <GL/glew.h>


namespace GraphicEngine::OpenGL
{
	class Texture2D
	{
	public:
		Texture2D(const std::string& path);
		void use();
		void use(int layer);
	private:
		int width{ 0 };
		int height{ 0 };
		int channels{ 0 };

		GLuint texture;

		static constexpr std::array<GLenum, 4> internalFormats = { {GL_R, GL_RG, GL_RGB, GL_RGBA} };
	};
}

