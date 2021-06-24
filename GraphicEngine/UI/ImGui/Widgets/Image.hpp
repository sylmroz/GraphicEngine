#pragma once

#pragma once

#include "../../../Common/Widget.hpp"
#include <imgui.h>

namespace GraphicEngine::GUI
{
	class Image : public Widget
	{
	public:
		Image(uint32_t texId, uint32_t width, uint32_t height);

		// Inherited via Widget
		virtual void draw() override;

		void setTextureId(uint32_t texId);
	private:
		uint32_t m_texId;
		uint32_t m_width;
		uint32_t m_height;
	};
}