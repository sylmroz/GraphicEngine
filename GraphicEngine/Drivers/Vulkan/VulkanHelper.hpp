#ifndef GRAPHIC_ENGINE_UTILS_VULKAN_HELPER_HPP
#define GRAPHIC_ENGINE_UTILS_VULKAN_HELPER_HPP

#include <array>
#include <optional>
#include <set>
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
	}

	template <typename F, typename... Args>
	void singleTimeCommand(const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, const vk::Queue& graphicQueue, F fun, Args... args)
	{
		auto commandBuffer = std::move(
			device->allocateCommandBuffersUnique(
				vk::CommandBufferAllocateInfo(commandPool.get(), vk::CommandBufferLevel::ePrimary, 1)).front());
		singleTimeCommand(commandBuffer, commandPool, graphicQueue, fun, args...);
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

	class DepthBufferData : public ImageData 
	{
	public:
		DepthBufferData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits numOfSamples);
	};

	class BufferData
	{
	public:
		
		BufferData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device,
		           const vk::BufferUsageFlags& usageFlags, const vk::MemoryPropertyFlags& properties, uint32_t size);

		vk::UniqueBuffer buffer;
		vk::UniqueDeviceMemory memory;
	};

	template <typename T>
	class UniformBuffer
	{
		UniformBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, uint32_t count)
		{
			_device = device;;
			for (uint32_t i{ 0 }; i < count; ++i)
			{
				_bufferData.emplace_back(BufferData(physicalDevice, device, vk::BufferUsageFlagBits::eUniformBuffer,
					vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, sizeof(T)));
			}
		}
		
		void setValue(T value)
		{
			_value = value;
		}

		void update(uint32_t bufferIndex)
		{
			copyMemoryToDevice<T>(_device, _bufferData[bufferIndex].memory, &_value, 1);
		}

		void updateAndSet(T value, uint32_t bufferIndex)
		{
			setValue(value);
			update(bufferIndex);
		}
	private:
		std::vector<BufferData> _bufferData;
		T _value;
		vk::UniqueDevice _device;
	};

	template <typename T>
	class DeviceBufferData
	{
	public:
		DeviceBufferData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue,
			const vk::BufferUsageFlags& usageFlags, const T* data, uint32_t size)
		{
			BufferData stagingBuffer(physicalDevice, device, usageFlags, vk::MemoryPropertyFlagBits::eHostVisible|vk::MemoryPropertyFlagBits::eHostCoherent, size);
			copyMemoryToDevice<T>(device, stagingBuffer.memory, data, size);
			buffer = std::make_unique<BufferData>(physicalDevice, device, usageFlags, vk::MemoryPropertyFlagBits::eDeviceLocal, size);
			singleTimeCommand(device, commandPool, queue, [&](const vk::UniqueCommandBuffer& commandBuffer)
				{
					commandBuffer->copyBuffer(stagingBuffer.buffer.get(), buffer->buffer.get(), vk::BufferCopy(0, 0, sizeof(T) * size));
				});
		}

		std::unique_ptr<BufferData> buffer;
	};

	class IndicesDeviceBuffer : public DeviceBufferData<uint32_t>
	{
	public:
		IndicesDeviceBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue,
			const vk::BufferUsageFlags& usageFlags, const vk::MemoryPropertyFlags& properties,const std::vector<uint32_t>& indices):
			DeviceBufferData(physicalDevice, device, commandPool, queue, vk::BufferUsageFlagBits::eIndexBuffer,indices.data(), indices.size()){}
	};

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

	vk::UniquePipeline createGraphicPipeline(const vk::UniqueDevice& device, const vk::UniquePipelineCache& pipeliceCache, const ShaderInfo& vertexShaderInfo, const ShaderInfo& fragmentShaderInfo,
		uint32_t vertexStride, std::vector<vk::VertexInputAttributeDescription> attributeDescriptions, const vk::VertexInputBindingDescription& bindingDescription,
		bool depthBuffered, const vk::FrontFace& frontFace, const vk::UniquePipelineLayout& pipelineLayout,
		const vk::UniqueRenderPass& renderPass, vk::SampleCountFlagBits msaaSample, bool depthBoundsTestEnable = false,
		bool stencilTestEnable = false);

	std::vector<vk::VertexInputAttributeDescription> createVertexInputAttributeDescriptions(const std::vector<std::pair<uint32_t, uint32_t>>& vertexSizeOffset);

}

#endif // !GRAPHIC_ENGINE_UTILS_VULKAN_HELPER_HPP

