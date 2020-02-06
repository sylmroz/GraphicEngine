#ifndef GRAPHIC_ENGINE_UTILS_VULKAN_HELPER_HPP
#define GRAPHIC_ENGINE_UTILS_VULKAN_HELPER_HPP

#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace GraphicEngine::Utils::Vulkan
{

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return (graphicsFamily.has_value() && presentFamily.has_value());
		}
	};

	QueueFamilyIndices findGraphicAndPresentQueueFamilyIndices(const vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface);

	vk::UniqueInstance createUniqueInstance(std::string appName = "",
		std::string engineName = "",
		std::vector<std::string> validationLayers = {},
		std::vector<std::string> extensionLayers = {},
		uint32_t apiVersion = VK_API_VERSION_1_0);

	bool isPhysicalDeviceSituable(const vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface);

	vk::PhysicalDevice getPhysicalDevice(const vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface);
}

#endif // !GRAPHIC_ENGINE_UTILS_VULKAN_HELPER_HPP

