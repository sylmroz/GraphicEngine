#ifndef GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP

#include "RenderingEngine.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanRenderingEngine : public GraphicEngine::RenderingEngine
	{
	public:
		virtual bool drawFrame();
		virtual void init();
	private:

	};
}

#endif // !GRAPHIC_ENGINE_VULKAN_RENDERING_ENGINE_HPP