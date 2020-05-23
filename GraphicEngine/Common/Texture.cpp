#include "Texture.hpp"

#ifdef USE_OPENCV

void GraphicEngine::Common::Details::OpenCVTexture::read(const std::string& path)
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

void GraphicEngine::Common::Details::StbTexture::read(const std::string& path)
{
	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data)
	{
		std::string err = "Cannot open image in path " + path;
		throw std::runtime_error(err);
	}
}
#endif // USE_STB_IMAGE
