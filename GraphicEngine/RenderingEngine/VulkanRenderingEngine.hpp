#ifndef GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP

#include "RenderingEngine.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanRenderingEngine : public GraphicEngine::RenderingEngine
	{
	public:
		virtual bool drawFrame() override;
		virtual void init(size_t width, size_t height) override;
		virtual void resizeFrameBuffer(size_t width, size_t height) override;
	private:

	};
}

#endif // !GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP