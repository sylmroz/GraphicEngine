#pragma once

#include <array>
#include <string>
#include <GL/glew.h>

namespace GraphicEngine::OpenGL
{
	class TextureCube
	{
	public:
		TextureCube(const std::array<std::string, 6>& faces);
		TextureCube(const std::array<std::tuple<uint8_t*, int, int, int>, 6>& faces);
		void use();
		void use(int layer);

		static auto getFormat(int channels)
		{
			return internalFormats.at(static_cast<size_t>(channels) - 1);
		}
	private:
		int width{ 0 };
		int height{ 0 };
		int channels{ 0 };

		GLuint texture;

		static constexpr std::array<GLenum, 4> internalFormats = { {GL_R, GL_RG, GL_RGB, GL_RGBA} };
	};
}
