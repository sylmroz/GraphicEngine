#pragma once

#include "VulkanFramework.hpp"
#include "../../Core/Utils/MememoryUtils.hpp"

namespace GraphicEngine::Vulkan
{
	class IUniformBuffer
	{
	public:
		IUniformBuffer(VulkanFramework* framework, vk::DescriptorType descriptorType): 
			m_descriptorType{descriptorType},
			m_framework{framework}
		{}

		vk::DescriptorType getDescriptorType()
		{
			return m_descriptorType;
		}

		virtual void update() = 0;

		std::vector<std::shared_ptr<BufferData>> bufferData;

	protected:
		vk::DescriptorType m_descriptorType;
		VulkanFramework* m_framework;
	};

	template <typename T>
	class UniformBuffer : public IUniformBuffer
	{
	public:
		UniformBuffer(VulkanFramework* framework) : IUniformBuffer(framework, vk::DescriptorType::eUniformBuffer)
		{
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

		virtual void update() override
		{
			copyMemoryToDevice<T>(m_framework->m_device, bufferData[m_framework->m_imageIndex.value]->memory, &m_value, 1);
		}

		void updateAndSet(T value)
		{
			setValue(value);
			update();
		}

	private:
		T m_value{};
	};

	template <typename T>
	class UniformBufferDynamic : public IUniformBuffer
	{
	public:
		UniformBufferDynamic(VulkanFramework* framework, uint32_t instances) : IUniformBuffer(framework, vk::DescriptorType::eUniformBufferDynamic)
		{
			m_aligmentSize = getDynamicAligmentSize<T>(framework->m_physicalDevice);
			m_values.resize(instances * m_aligmentSize);
			for (uint32_t i{ 0 }; i < framework->m_maxFrames; ++i)
			{
				bufferData.emplace_back(std::make_shared<BufferData>(m_framework->m_physicalDevice, m_framework->m_device, vk::BufferUsageFlagBits::eUniformBuffer,
					vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, instances * m_aligmentSize));
			}
		}

		void setValue(const std::vector<T>& values)
		{
			Core::Utils::copyValuesToAlignedBuffer<std::vector, T>(values, m_values, m_aligmentSize);
		}

		virtual void update() override
		{
			copyMemoryToDevice<char>(m_framework->m_device, bufferData[m_framework->m_imageIndex.value]->memory, m_values.data(), m_values.size());
		}

		void updateAndSet(const std::vector<T>& values)
		{
			setValue(values);
			update();
		}

	private:
		std::vector<char> m_values;
		uint32_t m_aligmentSize;
	};
}
