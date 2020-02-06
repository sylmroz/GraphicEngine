#include "VulkanHelper.hpp"
#include <iomanip>
#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	if (pCallbackData != nullptr)
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

GraphicEngine::Utils::Vulkan::QueueFamilyIndices GraphicEngine::Utils::Vulkan::findGraphicAndPresentQueueFamilyIndices(const vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface)
{
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

	QueueFamilyIndices indices;

	auto getGraphicQueueFamilyIndex = [&]()
	{
		uint32_t tempGraphicQueueFamilyIndex = std::distance(std::begin(queueFamilyProperties),
			std::find_if(std::begin(queueFamilyProperties), std::end(queueFamilyProperties),
				[](vk::QueueFamilyProperties prop) {return prop.queueFlags & vk::QueueFlagBits::eGraphics; }));
		return tempGraphicQueueFamilyIndex < queueFamilyProperties.size() ? std::optional<uint32_t>{tempGraphicQueueFamilyIndex} : std::nullopt;
	};

	indices.graphicsFamily = getGraphicQueueFamilyIndex();

	if (indices.graphicsFamily.has_value())
	{
		indices.presentFamily = physicalDevice.getSurfaceSupportKHR(indices.graphicsFamily.value(), surface.get()) == VK_TRUE ? indices.graphicsFamily : std::nullopt;
		if (!indices.presentFamily.has_value())
		{


			uint32_t tempGraphicQueueFamilyIndex = std::distance(std::begin(queueFamilyProperties),
				std::find_if(std::begin(queueFamilyProperties), std::end(queueFamilyProperties),
					[&](vk::QueueFamilyProperties prop) 
					{
						return prop.queueFlags & vk::QueueFlagBits::eGraphics && physicalDevice.getSurfaceSupportKHR(indices.graphicsFamily.value(), surface.get()); 
					}));

			if (tempGraphicQueueFamilyIndex < queueFamilyProperties.size())
			{
				indices.graphicsFamily = tempGraphicQueueFamilyIndex;
				indices.presentFamily = tempGraphicQueueFamilyIndex;
			}

			else
			{
				for (size_t i{ 0 }; i < queueFamilyProperties.size(); ++i)
				{
					if (physicalDevice.getSurfaceSupportKHR(i, surface.get()))
					{
						indices.presentFamily = i;
						break;
					}
				}
			}
		}
	}
	return indices;
}

vk::UniqueInstance GraphicEngine::Utils::Vulkan::createUniqueInstance(std::string appName, std::string engineName, std::vector<std::string> validationLayers, std::vector<std::string> extensionLayers, uint32_t apiVersion)
{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
	static vk::DynamicLoader dl;
#endif

#if !defined(NDEBUG)
	auto layerProperties = vk::enumerateInstanceLayerProperties();
	auto extensionProperties = vk::enumerateInstanceExtensionProperties();
#endif

	std::vector<const char*> requiredLayers;

	for (auto const& layer : validationLayers)
	{
		if (std::find_if(std::begin(layerProperties), std::end(layerProperties), [layer](auto layerProperty) { return layer == layerProperty.layerName; }) == std::end(layerProperties))
		{
			throw std::runtime_error("Failed to match validation layers!");
		}
		requiredLayers.push_back(layer.c_str());
	}
	
	std::vector<const char*> extensions;
	for (auto const& extension : extensionLayers)
	{
		if (std::find_if(std::begin(extensionProperties), std::end(extensionProperties), [extension](auto extensionProperty) { return extension == extensionProperty.extensionName; }) == std::end(extensionProperties))
		{
			throw std::runtime_error("Failed to match extension layers!");
		}
		extensions.push_back(extension.c_str());
	}

#if !defined(NDEBUG)
	if (std::find(std::begin(extensionLayers), std::end(extensionLayers), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == std::end(extensionLayers))
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
#endif
	vk::ApplicationInfo appInfo(appName.c_str(), VK_MAKE_VERSION(1, 0, 0), engineName.c_str(), VK_MAKE_VERSION(1, 0, 0), apiVersion);

#if defined(NDEBUG)
	vk::StructureChain<vk::InstanceCreateInfo> createInfo({ {}, &appInfo, requiredLayers.size(), requiredLayers.data(), extensions.size(), extensions.data() });
#else
	vk::DebugUtilsMessageSeverityFlagsEXT severityFlag(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);

	vk::DebugUtilsMessageTypeFlagsEXT typeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

	vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> createInfo(
		{ {}, &appInfo, static_cast<uint32_t>(requiredLayers.size()), requiredLayers.data(), static_cast<uint32_t>(extensions.size()), extensions.data() },
		{ {}, severityFlag, typeFlags, debugCallback });
#endif // !DEBUG

	vk::UniqueInstance instance = vk::createInstanceUnique(createInfo.get<vk::InstanceCreateInfo>());

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
	VULKAN_HPP_DISPATCH_LOADER_DYNAMIC.init(*instance);
#endif
	
	return instance;
}

bool GraphicEngine::Utils::Vulkan::isPhysicalDeviceSituable(const vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface)
{
	QueueFamilyIndices indices = findGraphicAndPresentQueueFamilyIndices(physicalDevice, surface);

	return indices.isComplete();
}

vk::PhysicalDevice GraphicEngine::Utils::Vulkan::getPhysicalDevice(const vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface)
{
	auto physicalDevices = instance->enumeratePhysicalDevices();

	for (auto physicalDevice : physicalDevices)
	{
		if (isPhysicalDeviceSituable(physicalDevice, surface))
		{
			return physicalDevice;
		}
	}

	return vk::PhysicalDevice();
}
