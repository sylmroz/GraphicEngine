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
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormats[channels - 1], width, height, 0, internalFormats[channels - 1], GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	catch (std::runtime_error& err)
	{
		throw err;
	}
}

void GraphicEngine::OpenGL::Texture2D::use()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

void GraphicEngine::OpenGL::Texture2D::use(int layer)
{
	glActiveTexture(GL_TEXTURE0 + layer);
	glBindTexture(GL_TEXTURE_2D, texture);
}
