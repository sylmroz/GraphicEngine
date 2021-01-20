#include "OpenGLTexture.hpp"
#include "../../Common/TextureReader.hpp"
#include <stdexcept>

constexpr std::array<GLenum, 4> GraphicEngine::OpenGL::Texture2D::internalFormats;

GraphicEngine::OpenGL::Texture2D::Texture2D(const std::string& path)
{
	try
	{
		Common::TextureReader textureReader(path);
		auto [data, w, h, c] = textureReader();
		width = w;
		height = h;
		channels = c;
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, getFormat(channels), width, height, 0, getFormat(channels), GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	catch (std::runtime_error& err)
	{
		throw err;
	}
}

GraphicEngine::OpenGL::Texture2D::Texture2D(const uint8_t* data, int width, int height, int channels)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, getFormat(channels), width, height, 0, getFormat(channels), GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void GraphicEngine::OpenGL::Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

void GraphicEngine::OpenGL::Texture::use(int layer)
{
	glActiveTexture(GL_TEXTURE0 + layer);
	glBindTexture(GL_TEXTURE_2D, texture);
}

GLuint GraphicEngine::OpenGL::Texture::getTexture()
{
	return texture;
}

GraphicEngine::OpenGL::TextureDepth::TextureDepth(int width, int height)
{
	this->width = width;
	this->height = height;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
}
