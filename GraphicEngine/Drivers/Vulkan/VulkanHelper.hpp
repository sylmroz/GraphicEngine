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

	class TextureData : public ImageData
	{

		// TODO
	};

	template <typename T>
	class UniformBuffer
	{
	public:
		UniformBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, uint32_t count)
		{
			for (uint32_t i{ 0 }; i < count; ++i)
			{
				bufferData.emplace_back(std::make_shared<BufferData>(physicalDevice, device, vk::BufferUsageFlagBits::eUniformBuffer,
					vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, sizeof(T)));
			}
		}

		void setValue(T value)
		{
			m_value = value;
		}

		void update(const vk::UniqueDevice& device, uint32_t bufferIndex)
		{
			copyMemoryToDevice<T>(device, bufferData[bufferIndex]->memory, &m_value, 1);
		}

		void updateAndSet(const vk::UniqueDevice& device, T value, uint32_t bufferIndex)
		{
			setValue(value);
			update(device, bufferIndex);
		}

		std::vector<std::shared_ptr<BufferData>> bufferData;

	private:
		T m_value;
	};

	template <typename T>
	class DeviceBufferData
	{
	public:
		DeviceBufferData(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue,
			const vk::BufferUsageFlags& usageFlags, const T* data, uint32_t size)
		{
			BufferData stagingBuffer(physicalDevice, device, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, sizeof(T) * size);
			copyMemoryToDevice<T>(device, stagingBuffer.memory, data, size);
			buffer = std::make_unique<BufferData>(physicalDevice, device, usageFlags, vk::MemoryPropertyFlagBits::eDeviceLocal, sizeof(T) * size);
			singleTimeCommand(device, commandPool, queue, [&](const vk::UniqueCommandBuffer& commandBuffer)
				{
					commandBuffer->copyBuffer(stagingBuffer.buffer.get(), buffer->buffer.get(), vk::BufferCopy(0, 0, sizeof(T) * size));
				});
		}

		virtual ~DeviceBufferData() = default;

		std::unique_ptr<BufferData> buffer;
	};

	class IndicesDeviceBuffer : public DeviceBufferData<uint32_t>
	{
	public:
		IndicesDeviceBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue,
			const std::vector<uint32_t>& indices) :
			DeviceBufferData(physicalDevice, device, commandPool, queue,
				vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst, indices.data(), indices.size()) {}
	};

	template <typename _Vertex>
	class VertexDeviceBuffer : public DeviceBufferData<_Vertex>
	{
	public:
		VertexDeviceBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue,
			const std::vector<_Vertex>& vertices) :
			DeviceBufferData<_Vertex>(physicalDevice, device, commandPool, queue,
				vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vertices.data(), vertices.size()) {}
	};

	template <typename _Vertex>
	class VertexBuffer
	{
		class _VertexBuffer
		{
		public:
			_VertexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::vector<_Vertex>& vertices)
			{
				_size = vertices.size();
				_vertexArrayObject = std::make_unique<VertexDeviceBuffer<_Vertex>>(physicalDevice, device, commandPool, queue, vertices);
			}

			virtual void bind(const vk::UniqueCommandBuffer& commandBuffer)
			{
				commandBuffer->bindVertexBuffers(0, this->_vertexArrayObject->buffer->buffer.get(), { 0 });
			}

			virtual void draw(const vk::UniqueCommandBuffer& commandBuffer)
			{
				commandBuffer->draw(_size, 1, 0, 0);
			}
			virtual ~_VertexBuffer() = default;
		protected:
			std::unique_ptr<VertexDeviceBuffer<_Vertex>> _vertexArrayObject;
			uint32_t _size;
		};


		class _VertexBufferWithIndices : public _VertexBuffer
		{
		public:
			_VertexBufferWithIndices(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::vector<_Vertex>& vertices, const std::vector<uint32_t>& indices) :
				_VertexBuffer(physicalDevice, device, commandPool, queue, vertices)
			{
				this->_size = indices.size();
				_indicesDeviceBuffer = std::make_unique<IndicesDeviceBuffer>(physicalDevice, device, commandPool, queue, indices);
			}

			virtual void bind(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				commandBuffer->bindVertexBuffers(0, this->_vertexArrayObject->buffer->buffer.get(), { 0 });
				commandBuffer->bindIndexBuffer(_indicesDeviceBuffer->buffer->buffer.get(), 0, vk::IndexType::eUint32);
			}

			virtual void draw(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				commandBuffer->drawIndexed(this->_size, 1, 0, 0, 0);
			}

			virtual ~_VertexBufferWithIndices() = default;

		private:
			std::unique_ptr<IndicesDeviceBuffer> _indicesDeviceBuffer;
		};
	public:
		VertexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::vector<_Vertex>& vertices)
		{
			_data = std::make_unique<_VertexBuffer>(physicalDevice, device, commandPool, queue, vertices);
		}

		VertexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::vector<_Vertex>& vertices, const std::vector<uint32_t>& indices)
		{
			_data = std::make_unique<_VertexBufferWithIndices>(physicalDevice, device, commandPool, queue, vertices, indices);
		}

		void bind(const vk::UniqueCommandBuffer& commandBuffer)
		{
			_data->bind(commandBuffer);
		}

		void draw(const vk::UniqueCommandBuffer& commandBuffer)
		{
			_data->draw(commandBuffer);
		}
	private:
		std::unique_ptr<_VertexBuffer> _data;
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
		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions, const vk::VertexInputBindingDescription& bindingDescription,
		bool depthBuffered, const vk::FrontFace& frontFace, const vk::UniquePipelineLayout& pipelineLayout,
		const vk::UniqueRenderPass& renderPass, vk::SampleCountFlagBits msaaSample, bool depthBoundsTestEnable = false,
		bool stencilTestEnable = false);

	std::vector<vk::VertexInputAttributeDescription> createVertexInputAttributeDescriptions(const std::vector<std::pair<uint32_t, uint32_t>>& vertexSizeOffset);

	vk::UniqueDescriptorPool createDescriptorPool(const vk::UniqueDevice& device, const std::vector<vk::DescriptorPoolSize>& descriptorSizes);

	vk::UniqueDescriptorSetLayout createDescriptorSetLayout(const vk::UniqueDevice& device,
		const std::vector<std::tuple<vk::DescriptorType, uint32_t, vk::ShaderStageFlags>>& bindingData, vk::DescriptorSetLayoutCreateFlags flags);

	void updateDescriptorSets(const vk::UniqueDevice& device, const vk::UniqueDescriptorPool& descriptorPool, const vk::UniqueDescriptorSetLayout& descriptorSetLayout, uint32_t layoutCount,
		const std::vector<vk::UniqueDescriptorSet>& descriptorSets, const std::vector<std::vector<std::shared_ptr<BufferData>>>& uniformBuffers, const std::vector<std::pair<vk::UniqueImageView, vk::UniqueSampler>>& imageUniforms);

}
