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
		Texture2D(const uint8_t* data, int width, int height, int channels);
		void use();
		void use(int layer);

		static auto getFormat(int channels)
		{
			return internalFormats.at(static_cast<size_t>(channels) - 1);
		}
	private:
		int width{ 0 };
		int height{ 0 };
		int channels{ 0 };

		GLuint texture;

		static constexpr std::array<GLenum, 4> internalFormats = { {GL_R, GL_RG, GL_RGB, GL_RGBA} };
	};
}

