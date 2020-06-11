#pragma once

#include "../../Common/TextureFactory.hpp"
#include "../../Common/TextureReader.hpp"
#include "OpenGLTexture.hpp"

namespace GraphicEngine::OpenGL
{
	using TextureFactory = Common::TextureFactory<Common::TextureReader, Texture2D>;
}