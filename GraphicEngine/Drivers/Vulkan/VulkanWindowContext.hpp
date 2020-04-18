#pragma once

#include <vulkan/vulkan.hpp>

namespace GraphicEngine::Vulkan
{
	class VulkanWindowContext
	{
	public:
		virtual ~VulkanWindowContext() = default;
		virtual VkSurfaceKHR createSurface(const vk::UniqueInstance& instance) = 0;
		virtual std::vector<std::string> getRequiredExtensions() = 0;
	};
}
