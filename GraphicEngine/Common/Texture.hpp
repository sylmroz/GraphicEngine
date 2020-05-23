#pragma once

#ifndef USE_STB_IMAGE
#define USE_OPENCV
#endif // !USE_STB_IMAGE

#include <string>
#include <tuple>

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#endif // USE_OPENCV

namespace GraphicEngine::Common
{
	namespace Details
	{
		template <typename TextureReader>
		class TextureMixin : public TextureReader
		{
		public:
			TextureMixin() = default;
			TextureMixin(const std::string& path) : TextureReader(path) {}

			std::tuple<unsigned char*, int, int, int> operator()()
			{
				return std::make_tuple(TextureReader::data, TextureReader::width, TextureReader::height, TextureReader::channels);
			}

			std::tuple<unsigned char*, int, int, int> operator()(const std::string& path)
			{
				TextureReader::read(path);
				return std::make_tuple(TextureReader::data, TextureReader::width, TextureReader::height, TextureReader::channels);
			}
		};

		template <typename TextureReader>
		class TextureCRTP
		{
		public:
			TextureCRTP() = default;
			TextureCRTP(const std::string& path)
			{
				static_cast<TextureReader*>(this)->read(path);
			}

			virtual ~TextureCRTP()
			{
#ifdef USE_STB_IMAGE
				if (data != nullptr)
				{
					delete data;
				}
#endif // USE_STB_IMAGE
			}
		protected:
			unsigned char* data{ nullptr };
			int width{ 0 };
			int height{ 0 };
			int channels{ 0 };
		};

#ifdef USE_OPENCV

		class OpenCVTexture : public Details::TextureCRTP<OpenCVTexture>
		{
		public:
			OpenCVTexture() = default;
			OpenCVTexture(const std::string& path) : TextureCRTP(path) {}

			void read(const std::string& path);

		private:
			cv::Mat image;
		};
#endif // USE_OPENCV

#ifdef USE_STB_IMAGE
		class StbTexture : public Details::TextureCRTP<StbTexture>
		{
		public:
			StbTexture() = default;
			StbTexture(const std::string& path) : TextureCRTP(path) {}

			void read(const std::string& path);
		};

		using Texture = Details::TextureMixin<StbTexture>;
#endif // USE_STB_IMAGE
	}

#ifdef USE_OPENCV
	using Texture = Details::TextureMixin<Details::OpenCVTexture>;
#endif // USE_OPENCV
#ifdef USE_STB_IMAGE
	using Texture = Details::TextureMixin<Details::StbTexture>;
#endif // USE_STB_IMAGE
}