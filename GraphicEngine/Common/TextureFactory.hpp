#pragma once

#include <memory>
#include <string>

namespace GraphicEngine::Common
{
	template <typename TextureReader, typename Texture, typename... Args>
	class TextureFactory
	{
	public:
		static std::shared_ptr<Texture> produceTexture(const std::string& path, Args... args)
		{
			TextureReader textureReader(path);
			auto [data, width, height, channels] = textureReader();
			return std::make_shared<Texture>(args..., data, width, height, channels);
		}
	};
}