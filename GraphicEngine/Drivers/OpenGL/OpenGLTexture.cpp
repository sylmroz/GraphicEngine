#include "OpenGLTexture.hpp"
#include "../../Common/TextureReader.hpp"
#include <stdexcept>

constexpr std::array<GLenum, 4> GraphicEngine::OpenGL::Texture2D::internalFormats;

GraphicEngine::OpenGL::Texture2D::Texture2D(const std::string& path)
{
	textureType = GL_TEXTURE_2D;
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
	textureType = GL_TEXTURE_2D;
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
	glBindTexture(textureType, texture);
}

void GraphicEngine::OpenGL::Texture::use(int layer)
{
	glActiveTexture(GL_TEXTURE0 + layer);
	glBindTexture(textureType, texture);
}

GLuint GraphicEngine::OpenGL::Texture::getTexture()
{
	return texture;
}

int GraphicEngine::OpenGL::Texture::getWidth()
{
	return width;
}

int GraphicEngine::OpenGL::Texture::getHeight()
{
	return height;
}

GraphicEngine::OpenGL::TextureDepth::TextureDepth(int width, int height)
{
	textureType = GL_TEXTURE_2D;
	this->width = width;
	this->height = height;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

GraphicEngine::OpenGL::Texture2DArray::Texture2DArray(const std::string& path)
{
	textureType = GL_TEXTURE_2D_ARRAY;
}

GraphicEngine::OpenGL::Texture2DArray::Texture2DArray(const uint8_t* data, int width, int height, int layers, int channels)
{
	textureType = GL_TEXTURE_2D_ARRAY;
}

GraphicEngine::OpenGL::TextureDepthArray::TextureDepthArray(int width, int height, int layers)
{
	textureType = GL_TEXTURE_2D_ARRAY;
	this->width = width;
	this->height = height;
	this->layers = layers;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, width, height, layers, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);


	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

int GraphicEngine::OpenGL::TextureArray::getLayers()
{
	return this->layers;
}

GraphicEngine::OpenGL::TextureCubeArray::TextureCubeArray(int width, int height, int layers)
{
	textureType = GL_TEXTURE_CUBE_MAP_ARRAY;
}

GraphicEngine::OpenGL::TextureCubeDepthArray::TextureCubeDepthArray(int width, int height, int layers)
{
	textureType = GL_TEXTURE_CUBE_MAP_ARRAY;
	this->width = width;
	this->height = height;
	this->layers = layers;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, texture);

	glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT, width, height, layers * 6, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);


	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
