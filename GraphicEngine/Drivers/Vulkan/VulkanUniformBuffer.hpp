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

		virtual int size() = 0;

		std::vector<std::shared_ptr<BufferData>> bufferData;

	protected:
		void initializeBufferData(uint32_t size)
		{
			bufferData.clear();
			for (uint32_t i{ 0 }; i < m_framework->m_maxFrames; ++i)
			{
				bufferData.emplace_back(std::make_shared<BufferData>(m_framework->m_physicalDevice, m_framework->m_device, vk::BufferUsageFlagBits::eUniformBuffer,
					vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, size));
			}
		}
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
			initializeBufferData(sizeof(T));
		}

		void setValue(T value)
		{
			m_value = value;
		}

		virtual void update() override
		{
			copyMemoryToDevice<T>(m_framework->m_device, bufferData[m_framework->m_imageIndex.value]->memory, &m_value, 1);
		}

		virtual int size() override
		{
			return sizeof(T);
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
			resizeInstancesCount(instances);
		}

		void setValue(const std::vector<T>& values)
		{
			Core::Utils::copyValuesToAlignedBuffer<std::vector, T>(values, m_values, m_aligmentSize);
		}

		virtual void update() override
		{
			copyMemoryToDevice<char>(m_framework->m_device, bufferData[m_framework->m_imageIndex.value]->memory, m_values.data(), m_values.size());
		}

		int size() override
		{
			return m_values.size();
		}

		void updateAndSet(const std::vector<T>& values)
		{
			setValue(values);
			update();
		}

		void resizeInstancesCount(uint32_t instances)
		{
			m_instancesCount = instances;
			m_values.resize(m_instancesCount * m_aligmentSize);
			initializeBufferData(m_instancesCount * m_aligmentSize);
			/*if (m_instancesCount > m_bufferDataInstancesCount)
			{
				m_bufferDataInstancesCount += 500;
			}*/
		}

		void addInstance()
		{
			resizeInstancesCount(m_instancesCount + 1);
		}

		void deleteInstance()
		{
			--m_instancesCount;
		}

		uint32_t getInstancesCount()
		{
			return m_instancesCount;
		}

	private:
		std::vector<char> m_values;
		uint32_t m_aligmentSize;
		uint32_t m_instancesCount{ 0 };
		uint32_t m_bufferDataInstancesCount = 500;
	};
}
