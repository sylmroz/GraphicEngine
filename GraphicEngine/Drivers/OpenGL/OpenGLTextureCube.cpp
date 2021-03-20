#include "../../Common/TextureReader.hpp"
#include "OpenGLTextureCube.hpp"
#include <execution>
#include <mutex>
#include <stdexcept>

GraphicEngine::OpenGL::TextureCube::TextureCube(const std::array<std::string, 6>& faces)
{
	try
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);


		std::array<std::pair<std::string, uint32_t>, 6> facesWithIndexes;
		for (uint32_t index{ 0 }; index < 6; ++index)
		{
			facesWithIndexes[index] = std::make_pair(faces[index], index);
		}
		std::array<std::unique_ptr<Common::TextureReader>, 6> textureReaders;
		std::mutex m;
		std::for_each(std::execution::par, std::begin(facesWithIndexes), std::end(facesWithIndexes), [&](std::pair<std::string, uint32_t> path) {
			textureReaders[path.second] = std::make_unique<Common::TextureReader>(path.first);
			});

		uint32_t i{ 0 };

		for (auto& textureReader : textureReaders)
		{
			auto [data, w, h, c] = (*textureReader)();
			width = w;
			height = h;
			channels = c;
			if (data)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, getFormat(channels), width, height, 0, getFormat(channels), GL_UNSIGNED_BYTE, data);

			++i;
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	catch (std::runtime_error& err)
	{
		throw err;
	}
}

GraphicEngine::OpenGL::TextureCube::TextureCube(const std::array<std::tuple<uint8_t*, int, int, int>, 6>& faces)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	uint32_t i{ 0 };
	for (const auto& data : faces)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, getFormat(std::get<3>(data)), std::get<1>(data), std::get<2>(data), 0, getFormat(std::get<3>(data)), GL_UNSIGNED_BYTE, std::get<0>(data));
		++i;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void GraphicEngine::OpenGL::TextureCube::use()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

void GraphicEngine::OpenGL::TextureCube::use(int layer)
{
	glActiveTexture(GL_TEXTURE0 + layer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}
