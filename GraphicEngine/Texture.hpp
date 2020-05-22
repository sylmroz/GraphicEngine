#pragma once

#ifndef USE_STB_IMAGE
#define USE_OPENCV

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#endif // USE_OPENCV

#ifdef USE_STB_IMAGE
#include <stb_image.h>
#endif // USE_STB_IMAGE


namespace GraphicEngine::Common
{
	template <typename TextureCompiler>
	class Texture
	{
	public:
		template <typename... Args>
		Texture(char* data, int width, int height, int channels, Args... args)
		{
			textureCompiler = textureCompiler(data, width, height, channels, ...);
		}

		template <typename TextureReader, typename... Args>
		Texture(TextureReader& textureReader, Args... args)
		{
			textureCompiler = textureCompiler(textureReader, args...);
		}

		template <typename TextureReader, typename... Args>
		Texture(std::shared_ptr<TextureReader> textureReader, Args... args)
		{
			textureCompiler = textureCompiler(textureReader, args...);
		}

		TextureCompiler& texture() const
		{
			return textureCompiler;
		}

	private:
		TextureCompiler textureCompiler;
	};
}

#endif // !USE_STB_IMAGE