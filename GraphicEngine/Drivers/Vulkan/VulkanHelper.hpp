#pragma once

#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace GraphicEngine::Vulkan
{
	template <typename T>
	void copyMemoryToDevice(const vk::UniqueDevice& device, const vk::UniqueDeviceMemory& memory, const T* data, uint32_t count)
	{
		uint32_t deviceSize = sizeof(T) * count;
		void* _data;
		device->mapMemory(memory.get(), 0, deviceSize, vk::MemoryMapFlags(), &_data);
		memcpy(_data, data, deviceSize);
		device->unmapMemory(memory.get());
	}

	template <typename F, typename... Args>
	void singleTimeCommand(const vk::UniqueCommandBuffer& commandBuffer, const vk::UniqueCommandPool& commandPool, const vk::Queue& graphicQueue, F fun, Args... args)
	{
		commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
		fun(commandBuffer, args...);
		commandBuffer->end();
		graphicQueue.submit(vk::SubmitInfo(0, nullptr, nullptr, 1, &(commandBuffer.get())), nullptr);
		graphicQueue.waitIdle();
	}

	template <typename F, typename... Args>
	void singleTimeCommand(const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, const vk::Queue& graphicQueue, F fun, Args... args)
	{
		auto commandBuffer = std::move(
			device->allocateCommandBuffersUnique(
				vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1)).front());
		singleTimeCommand(commandBuffer, commandPool, graphicQueue, fun, args...);
	}

	template <typename T>
	uint32_t getDynamicAligmentSize(const vk::PhysicalDevice& device)
	{
		uint32_t minUboAlignment = device.getProperties().limits.minUniformBufferOffsetAlignment;
		uint32_t dynamicAlignment = sizeof(T);
		if (minUboAlignment > 0) {
			dynamicAlignment = (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
		}

		return dynamicAlignment;
	}

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return (graphicsFamily.has_value() && presentFamily.has_value());
		}
	};

	struct ShaderInfo
	{
		vk::ShaderModule shaderModule;
		vk::SpecializationInfo specializationInfo;
		vk::ShaderStageFlagBits shaderType;
	};

	struct SwapChainSupportDetails
	{
		SwapChainSupportDetails();
		SwapChainSupportDetails(vk::SurfaceCapabilitiesKHR capabilities, std::vector<vk::SurfaceFormatKHR> formats, std::vector<vk::PresentModeKHR> presentModes);
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	struct RenderingBarriers
	{
		RenderingBarriers(const vk::UniqueDevice& device, size_t maxFrames);
		std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
		std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
		std::vector<vk::UniqueFence> inFlightFences;
		std::vector<vk::Fence> imagesInFlight;
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

		SwapChainSupportDetails getSwapChainSupportDetails(const vk::PhysicalDevice& physicalDevice, const vk::UniqueSurfaceKHR& surface);

		vk::SurfaceFormatKHR pickSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
		vk::PresentModeKHR pickPresentMode(const std::vector<vk::PresentModeKHR>& presentModes);
		vk::Extent2D pickSurfaceExtent(const vk::SurfaceCapabilitiesKHR& capabilities, vk::Extent2D frameBufferExtent);
	};

	class BufferData
	{
	public:

		BufferData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device,
			const vk::BufferUsageFlags& usageFlags, const vk::MemoryPropertyFlags& properties, uint32_t size);
		virtual ~BufferData() = default;

		vk::UniqueDeviceMemory memory;
		vk::UniqueBuffer buffer;
	};

	class ImageData
	{
	public:
		ImageData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits numOfSamples,
			vk::MemoryPropertyFlags memoryProperty, vk::ImageUsageFlags imageUsage, vk::ImageTiling tiling,
			uint32_t mipLevel, vk::ImageLayout layout, vk::ImageAspectFlags aspectFlags, vk::ImageType imageType = vk::ImageType::e2D);

		vk::Format format;
		vk::UniqueDeviceMemory deviceMemory;
		vk::UniqueImage image;
		vk::UniqueImageView imageView;
	};

	class DepthBufferData : public ImageData
	{
	public:
		DepthBufferData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits numOfSamples);
	};

	class Texture2D;
	class IUniformBuffer;

	std::vector<const char*> getDeviceExtension();

	std::optional<uint32_t> getGraphicQueueFamilyIndex(const vk::PhysicalDevice& physicalDevice);

	QueueFamilyIndices findGraphicAndPresentQueueFamilyIndices(const vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface);

	vk::UniqueInstance createUniqueInstance(std::string appName = "",
		std::string engineName = "",
		std::vector<std::string> validationLayers = {},
		std::vector<std::string> extensionLayers = {},
		uint32_t apiVersion = VK_API_VERSION_1_0);

	bool isPhysicalDeviceSuitable(const vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface);

	vk::PhysicalDevice getPhysicalDevice(const vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface);

	vk::UniqueDevice getUniqueLogicalDevice(const vk::PhysicalDevice& physicalDevice, vk::UniqueSurfaceKHR& surface);

	vk::UniqueCommandPool createUniqueCommandPool(const vk::UniqueDevice& device, const QueueFamilyIndices& queueFamilyIndex);

	std::vector<vk::UniqueCommandBuffer> createUniqueCommandBuffers(const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, uint32_t commandCount = 1);

	uint32_t findMemoryType(const vk::PhysicalDevice& physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags memoryProperty);

	vk::UniqueDeviceMemory allocateMemory(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::MemoryPropertyFlags memoryProperty, const vk::MemoryRequirements& memoryRequirements);

	vk::UniqueRenderPass createRenderPass(const vk::UniqueDevice& device, vk::Format colorFormat, vk::Format depthFormat, vk::SampleCountFlagBits msaaSample);

	vk::Format findSupportedFormat(const vk::PhysicalDevice& physicalDevice, std::vector<vk::Format> candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags formatFeatures);

	vk::Format findDepthFormat(const vk::PhysicalDevice& physicalDevice);

	std::vector<vk::UniqueFramebuffer> createFrameBuffers(const vk::UniqueDevice& device, const vk::UniqueRenderPass& renderPass, vk::Extent2D extent, uint32_t layers,
		const std::vector<vk::UniqueImageView>& swapChainImageViews);

	std::vector<vk::UniqueFramebuffer> createFrameBuffers(const vk::UniqueDevice& device, const vk::UniqueRenderPass& renderPass, vk::Extent2D extent, uint32_t layers,
		const std::vector<vk::UniqueImageView>& swapChainImageViews, const vk::UniqueImageView& depthImageView);

	std::vector<vk::UniqueFramebuffer> createFrameBuffers(const vk::UniqueDevice& device, const vk::UniqueRenderPass& renderPass, vk::Extent2D extent, uint32_t layers,
		const vk::UniqueImageView& colorImageView, const vk::UniqueImageView& depthImageView, const std::vector<vk::UniqueImageView>& swapChainImageViews);

	vk::UniquePipeline createGraphicPipeline(const vk::UniqueDevice& device, const vk::UniquePipelineCache& pipeliceCache, const std::vector<ShaderInfo>& shadersInfo,
		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions, const vk::VertexInputBindingDescription& bindingDescription,
		bool depthBuffered, const vk::FrontFace& frontFace, const vk::UniquePipelineLayout& pipelineLayout,
		const vk::UniqueRenderPass& renderPass, vk::SampleCountFlagBits msaaSample, vk::CullModeFlags cullMode = vk::CullModeFlagBits::eNone, bool depthBoundsTestEnable = false,
		bool stencilTestEnable = false);

	std::vector<vk::VertexInputAttributeDescription> createVertexInputAttributeDescriptions(const std::vector<std::pair<uint32_t, uint32_t>>& vertexSizeOffset);

	vk::UniqueDescriptorPool createDescriptorPool(const vk::UniqueDevice& device, const std::vector<vk::DescriptorPoolSize>& descriptorSizes);

	vk::UniqueDescriptorSetLayout createDescriptorSetLayout(const vk::UniqueDevice& device,
		const std::vector<std::tuple<vk::DescriptorType, uint32_t, vk::ShaderStageFlags>>& bindingData, vk::DescriptorSetLayoutCreateFlags flags);

	void updateDescriptorSets(const vk::UniqueDevice& device, const vk::UniqueDescriptorPool& descriptorPool, const vk::UniqueDescriptorSetLayout& descriptorSetLayout, uint32_t layoutCount,
		const std::vector<vk::UniqueDescriptorSet>& descriptorSets, const std::vector<std::shared_ptr<IUniformBuffer>>& uniformBuffers, const std::vector<std::shared_ptr<Texture2D>>& imageUniforms);

	void transitionImageLayout(const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, const vk::Queue& graphicQueue, 
		vk::UniqueImage& image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t mipLevels);

	void generateMipmaps(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device,
		const vk::UniqueCommandPool& commandPool, const vk::Queue& queue,
		vk::UniqueImage& image, vk::Format format, int32_t width, int32_t height, int32_t mipLevels);
}
