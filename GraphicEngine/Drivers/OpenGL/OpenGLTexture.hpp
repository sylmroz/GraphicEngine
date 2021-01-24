#pragma once

#include <array>
#include <string>
#include <GL/glew.h>

namespace GraphicEngine::OpenGL
{

	class Texture
	{
	public:
		void use();
		void use(int layer);

		GLuint getTexture();

		int getWidth();
		int getHeight();

	protected:
		GLuint texture;
		int width{ 0 };
		int height{ 0 };
		int channels{ 0 };
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const std::string& path);
		Texture2D(const uint8_t* data, int width, int height, int channels);

		static auto getFormat(int channels)
		{
			return internalFormats.at(static_cast<size_t>(channels) - 1);
		}
	private:

		static constexpr std::array<GLenum, 4> internalFormats = { {GL_R, GL_RG, GL_RGB, GL_RGBA} };
	};

	class TextureDepth : public Texture
	{
	public:
		TextureDepth(int width, int height);
	};
}
