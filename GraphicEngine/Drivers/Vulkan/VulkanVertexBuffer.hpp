#pragma once

#include "VulkanHelper.hpp"
#include "../../Common/VertexBuffer.hpp"

namespace GraphicEngine::Vulkan
{
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
	class VertexBuffer : public Common::VertexBuffer<VertexBuffer<_Vertex>, _Vertex, vk::UniqueCommandPool&>
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

		void unbind(const vk::UniqueCommandBuffer& commandBuffer)
		{
			// Vulkan do not need unbinding method
		}
	private:
		std::unique_ptr<_VertexBuffer> _data;
	};
}