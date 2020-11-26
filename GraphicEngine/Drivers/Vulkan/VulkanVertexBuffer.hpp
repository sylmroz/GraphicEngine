#pragma once

#include "VulkanHelper.hpp"
#include "../../Common/VertexBuffer.hpp"

#include <stdexcept>

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
	class VertexBuffer : public Common::VertexBuffer<VertexBuffer<_Vertex>, vk::UniqueCommandPool&>
	{
		class _IVerexBuffer
		{
		public:
			virtual void bind(const vk::UniqueCommandBuffer& commandBuffer) = 0;

			virtual void bindSecond(const vk::UniqueCommandBuffer& commandBuffer) = 0;

			virtual void draw(const vk::UniqueCommandBuffer& commandBuffer) = 0;

			virtual void drawElements(const vk::UniqueCommandBuffer& commandBuffer) = 0;

			virtual void drawEdges(const vk::UniqueCommandBuffer& commandBuffer) = 0;

			virtual ~_IVerexBuffer() = default;
		};

		class _VertexBuffer : public _IVerexBuffer
		{
		public:
			_VertexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::vector<_Vertex>& vertices)
			{
				m_vertexBufferSize = vertices.size();
				m_vertexArrayObject = std::make_unique<VertexDeviceBuffer<_Vertex>>(physicalDevice, device, commandPool, queue, vertices);
			}

			virtual void bind(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				commandBuffer->bindVertexBuffers(0, this->m_vertexArrayObject->buffer->buffer.get(), { 0 });
			}

			virtual void bindSecond(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				throw std::logic_error("Function not yet implemented");
			}

			virtual void drawElements(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				throw std::logic_error("Function not yet implemented");
			}

			virtual void drawEdges(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				throw std::logic_error("Function not yet implemented");
			}

			virtual void draw(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				commandBuffer->draw(m_vertexBufferSize, 1, 0, 0);
			}

			virtual ~_VertexBuffer() = default;
		protected:
			std::unique_ptr<VertexDeviceBuffer<_Vertex>> m_vertexArrayObject;
			uint32_t m_vertexBufferSize;
		};


		class _VertexBufferWithIndices : public _VertexBuffer
		{
		public:
			_VertexBufferWithIndices(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::vector<_Vertex>& vertices, const std::vector<uint32_t>& indices) :
				_VertexBuffer(physicalDevice, device, commandPool, queue, vertices)
			{
				this->m_vertexBufferSize = vertices.size();
				m_indicesBufferSize = indices.size();
				m_indicesDeviceBuffer = std::make_unique<IndicesDeviceBuffer>(physicalDevice, device, commandPool, queue, indices);
			}

			virtual void bind(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				commandBuffer->bindVertexBuffers(0, this->m_vertexArrayObject->buffer->buffer.get(), { 0 });
				commandBuffer->bindIndexBuffer(m_indicesDeviceBuffer->buffer->buffer.get(), 0, vk::IndexType::eUint32);
			}

			virtual void drawElements(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				commandBuffer->drawIndexed(this->m_indicesBufferSize, 1, 0, 0, 0);
			}

			virtual void draw(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				commandBuffer->draw(this->m_vertexBufferSize, 1, 0, 0);
			}

			virtual ~_VertexBufferWithIndices() = default;

		private:
			std::unique_ptr<IndicesDeviceBuffer> m_indicesDeviceBuffer;
			uint32_t m_indicesBufferSize;
		};

		class _VertexBufferWithElementsAndEdges : public _IVerexBuffer
		{
		public:
			_VertexBufferWithElementsAndEdges() = default;
			_VertexBufferWithElementsAndEdges(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::vector<_Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& edges)
			{
				m_elements = std::make_unique<_VertexBufferWithIndices>(physicalDevice, device, commandPool, queue, vertices, indices);
				m_edges = std::make_unique<_VertexBufferWithIndices>(physicalDevice, device, commandPool, queue, vertices, edges);
			}
			virtual void bind(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				m_elements->bind(commandBuffer);
			}

			virtual void bindSecond(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				m_edges->bind(commandBuffer);
			}

			virtual void draw(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				m_elements->draw(commandBuffer);
			}

			virtual void drawElements(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				m_elements->bind(commandBuffer);
				m_elements->drawElements(commandBuffer);
			}

			virtual void drawEdges(const vk::UniqueCommandBuffer& commandBuffer) override
			{
				m_edges->bind(commandBuffer);
				m_edges->drawElements(commandBuffer);
			}

			virtual ~_VertexBufferWithElementsAndEdges() = default;

		private:
			std::unique_ptr<_VertexBufferWithIndices> m_elements;
			std::unique_ptr<_VertexBufferWithIndices> m_edges;
		};
	public:
		using VertexType = _Vertex;

		VertexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::vector<VertexType>& vertices)
		{
			m_data = std::make_unique<_VertexBuffer>(physicalDevice, device, commandPool, queue, vertices);
		}

		VertexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices)
		{
			m_data = std::make_unique<_VertexBufferWithIndices>(physicalDevice, device, commandPool, queue, vertices, indices);
		}

		VertexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, const vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& edges)
		{
			m_data = std::make_unique<_VertexBufferWithElementsAndEdges>(physicalDevice, device, commandPool, queue, vertices, indices, edges);
		}

		void bind(const vk::UniqueCommandBuffer& commandBuffer)
		{
			m_data->bind(commandBuffer);
		}

		void draw(const vk::UniqueCommandBuffer& commandBuffer)
		{
			m_data->draw(commandBuffer);
		}

		void drawElements(const vk::UniqueCommandBuffer& commandBuffer)
		{
			m_data->drawElements(commandBuffer);
		}

		void drawEdges(const vk::UniqueCommandBuffer& commandBuffer)
		{
			m_data->drawEdges(commandBuffer);
		}

		void unbind(const vk::UniqueCommandBuffer& commandBuffer)
		{
			// Vulkan do not need unbinding method
		}
	private:
		std::unique_ptr<_IVerexBuffer> m_data;
	};
}