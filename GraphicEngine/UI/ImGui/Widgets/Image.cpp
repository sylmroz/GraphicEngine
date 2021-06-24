#include "Image.hpp"

GraphicEngine::GUI::Image::Image(uint32_t texId, uint32_t width, uint32_t height):
	m_texId{texId}, m_width{ width }, m_height{ height }
{
}

void GraphicEngine::GUI::Image::draw()
{
	ImGui::Image((ImTextureID)m_texId, ImVec2{128.0f, 128.0f });
}

void GraphicEngine::GUI::Image::setTextureId(uint32_t texId)
{
	m_texId = texId;
}
