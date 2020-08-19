#pragma once

#include "VulkanFramework.hpp"

namespace GraphicEngine::Vulkan
{
	template <typename T>
	class UniformBuffer
	{
	public:
		UniformBuffer(VulkanFramework* framework)
		{
			m_framework = framework;
			for (uint32_t i{ 0 }; i < framework->m_maxFrames; ++i)
			{
				bufferData.emplace_back(std::make_shared<BufferData>(m_framework->m_physicalDevice, m_framework->m_device, vk::BufferUsageFlagBits::eUniformBuffer,
					vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, sizeof(T)));
			}
		}

		void setValue(T value)
		{
			m_value = value;
		}

		void update()
		{
			copyMemoryToDevice<T>(m_framework->m_device, bufferData[m_framework->m_imageIndex.value]->memory, &m_value, 1);
		}

		void updateAndSet(T value)
		{
			setValue(value);
			update();
		}

		std::vector<std::shared_ptr<BufferData>> bufferData;

	private:
		T m_value{};
		VulkanFramework* m_framework;
	};
}
