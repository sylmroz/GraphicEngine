#ifndef GRAPHIC_ENGINE_VULKAN_VULKAN_WINDOW_CONTEXT_HPP
#define GRAPHIC_ENGINE_VULKAN_VULKAN_WINDOW_CONTEXT_HPP

#include <vulkan/vulkan.hpp>

namespace GraphicEngine::Vulkan
{
	class VulkanWindowContext
	{
	public:
		virtual VkSurfaceKHR createSurface(const vk::UniqueInstance& instance) = 0;
		virtual std::vector<std::string> getRequiredExtensions() = 0;
	};
}

#endif // !GRAPHIC_ENGINE_VULKAN_VULKAN_WINDOW_CONTEXT_HPP
