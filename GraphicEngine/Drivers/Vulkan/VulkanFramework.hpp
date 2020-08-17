#pragma once

#include "VulkanHelper.hpp"
#include "VulkanWindowContext.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanFramework
	{
	public:
		VulkanFramework() = default;
		VulkanFramework(std::shared_ptr<VulkanWindowContext> vulkanWindowsContext, const std::string& appName, 
			const std::string& engineName, int width, int height, vk::SampleCountFlagBits msaaSamples,
			const std::vector<std::string>& validationLayers);

		VulkanFramework& initialize(std::shared_ptr<VulkanWindowContext> vulkanWindowsContext, const std::string& appName,
			const std::string& engineName, int width, int height, vk::SampleCountFlagBits msaaSamples,
			const std::vector<std::string>& validationLayers);

		virtual ~VulkanFramework() = default;
	protected:
	private:
		std::shared_ptr<VulkanWindowContext> m_vulkanWindowContext;

		vk::UniqueInstance m_instance;
		vk::PhysicalDevice m_physicalDevice;
		vk::UniqueSurfaceKHR m_surface;
		vk::UniqueDevice m_device;
		vk::Queue m_graphicQueue;
		vk::Queue m_presentQueue;

	private:
		std::string m_appName;
		std::string m_engineName;
		int m_width;
		int m_height;

	private:
		vk::SampleCountFlagBits m_msaaSamples;
		uint32_t m_maxFrames{ 1 };
		uint32_t m_currentFrameIndex{ 0 };
		QueueFamilyIndices m_indices;

		std::shared_ptr<VulkanWindowContext> m_vulkanWindowContext;
	private:
		std::vector<std::string> m_validationLayers;
	};
}
