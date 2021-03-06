#include "TextureReader.hpp"

#ifdef USE_OPENCV

void GraphicEngine::Common::Details::OpenCVTextureReder::read(const std::string& path)
{
	image = cv::imread(path);
	if (!image.data)
	{
		std::string err = "Cannot open image in path " + path;
		throw std::runtime_error(err);
	}

	data = image.data;
	width = image.cols;
	height = image.rows;
	channels = image.channels();
}
#endif // USE_OPENCV

#ifdef USE_STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>
#include <stdexcept>
#include <string>

void GraphicEngine::Common::Details::StbTextureReader::read(const std::string& path)
{
	data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (channels == 3)
	{
		channels++;
	}
	if (!data)
	{
		std::string err = "Cannot open image in path " + path;
		throw std::runtime_error(err);
	}
}
GraphicEngine::Common::Details::StbTextureReader::~StbTextureReader()
{
	stbi_image_free(data);
}
#endif // USE_STB_IMAGE
