#pragma once

#include "VulkanUniformBuffer.hpp"

namespace GraphicEngine::Vulkan
{
	template <typename T>
	class ShaderStorageBufferObject : public IUniformBuffer
	{
	public:
		ShaderStorageBufferObject(VulkanFramework* framework, uint32_t instances = 1) : IUniformBuffer{ framework, vk::DescriptorType::eStorageBuffer, vk::BufferUsageFlagBits::eStorageBuffer }, m_instances{ instances }
		{
			initializeBufferData(sizeof(T) * instances + 4 * sizeof(float));
		}
		// Inherited via IUniformBuffer
		virtual void update() override
		{
			// TODO
		}
		void update(const std::vector<T>& data)
		{
			uint32_t count = data.size();
			copyMemoryToDevice<uint32_t>(m_framework->m_device, bufferData[m_framework->m_imageIndex.value]->memory, &count, 1);
			copyMemoryToDevice<T>(m_framework->m_device, bufferData[m_framework->m_imageIndex.value]->memory, &data[0], data.size(), 4 * sizeof(float));
		}
		virtual int size()
		{
			return m_instances;
		}

	private:
		uint32_t m_instances;
	};
}