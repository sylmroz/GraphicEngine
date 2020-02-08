#ifndef GRAPHIC_ENGINE_UTILS_VULKAN_HELPER_HPP
#define GRAPHIC_ENGINE_UTILS_VULKAN_HELPER_HPP

#include <optional>
#include <set>
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

	struct SwapChainSupportDetails
	{
		SwapChainSupportDetails();
		SwapChainSupportDetails(vk::SurfaceCapabilitiesKHR capabilities, std::vector<vk::SurfaceFormatKHR> formats, std::vector<vk::PresentModeKHR> presentModes);
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	class SwapChainData
	{
	public:
		SwapChainData() {};
		SwapChainData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueSurfaceKHR& surface,
			const QueueFamilyIndices& indices, const vk::Extent2D& extend, const vk::UniqueSwapchainKHR& oldSwapChain, vk::ImageUsageFlags imageUsage);

		vk::Extent2D extent;
		vk::Format format;
		vk::UniqueSwapchainKHR swapChain;
		std::vector<vk::Image> images;
		std::vector<vk::UniqueImageView> imageViews;

	private:
		void createSwapChainData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueSurfaceKHR& surface,
			const QueueFamilyIndices& indices, const vk::Extent2D& extend, const vk::UniqueSwapchainKHR& oldSwapChain, vk::ImageUsageFlags imageUsage);

		SwapChainSupportDetails getSwapChainSuppordDetails(const vk::PhysicalDevice& physicalDevice, const vk::UniqueSurfaceKHR& surface);

		vk::SurfaceFormatKHR pickSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
		vk::PresentModeKHR pickPresentMode(const std::vector<vk::PresentModeKHR>& presentModes);
		vk::Extent2D pickSurfaceExtent(const vk::SurfaceCapabilitiesKHR& capabilities, vk::Extent2D frameBufferExtent);
	};

	class ImageData
	{
	public:
		ImageData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits numOfSamples,
			vk::MemoryPropertyFlags memoryProperty, vk::ImageUsageFlags imageUsage, vk::ImageTiling tiling,
			uint32_t mipLevel, vk::ImageLayout layout, vk::ImageAspectFlags aspectFlags);

		vk::Format format;
		vk::UniqueDeviceMemory deviceMemory;
		vk::UniqueImage image;
		vk::UniqueImageView imageView;
	};

	class DeepBufferData : public ImageData 
	{
	public:
		DeepBufferData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits numOfSamples);
	};

	std::vector<const char*> getDeviceExtension();

	std::optional<uint32_t> getGraphicQueueFamilyIndex(const vk::PhysicalDevice& physicalDevice);

	QueueFamilyIndices findGraphicAndPresentQueueFamilyIndices(const vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface);

	vk::UniqueInstance createUniqueInstance(std::string appName = "",
		std::string engineName = "",
		std::vector<std::string> validationLayers = {},
		std::vector<std::string> extensionLayers = {},
		uint32_t apiVersion = VK_API_VERSION_1_0);

	bool isPhysicalDeviceSituable(const vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface);

	vk::PhysicalDevice getPhysicalDevice(const vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface);

	vk::UniqueDevice getUniqueLogicalDevice(const vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface);

	vk::UniqueCommandPool createUniqueCommandPool(const vk::UniqueDevice& device, const QueueFamilyIndices& queueFamilyIndex);

	std::vector<vk::UniqueCommandBuffer> createUniqueCommandBuffers(const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool);

	uint32_t findMemoryType(const vk::PhysicalDevice& physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags memoryProperty);

	vk::UniqueDeviceMemory allocateMemory(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::MemoryPropertyFlags memoryProperty, const vk::MemoryRequirements& memoryRequirements);
}

#endif // !GRAPHIC_ENGINE_UTILS_VULKAN_HELPER_HPP

