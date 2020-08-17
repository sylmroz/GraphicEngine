#include "VulkanFramework.hpp"

GraphicEngine::Vulkan::VulkanFramework::VulkanFramework(std::shared_ptr<VulkanWindowContext> vulkanWindowsContext, const std::string& appName, const std::string& engineName, int width, int height, vk::SampleCountFlagBits msaaSamples, const std::vector<std::string>& validationLayers)
{
	initialize(vulkanWindowsContext, appName, engineName, width, height, msaaSamples, validationLayers);
}

GraphicEngine::Vulkan::VulkanFramework& GraphicEngine::Vulkan::VulkanFramework::initialize(std::shared_ptr<VulkanWindowContext> vulkanWindowsContext, const std::string& appName, const std::string& engineName, int width, int height, vk::SampleCountFlagBits msaaSamples, const std::vector<std::string>& validationLayers)
{
	m_vulkanWindowContext = vulkanWindowsContext;
	m_appName = appName;
	m_engineName = engineName;
	m_width = width;
	m_height = height;
	m_msaaSamples = msaaSamples;
	m_validationLayers = validationLayers;

	m_instance = createUniqueInstance(m_appName, m_engineName, m_validationLayers, m_vulkanWindowContext->getRequiredExtensions(), VK_API_VERSION_1_1);
	{
		auto surface = m_vulkanWindowContext->createSurface(m_instance);
		vk::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> _deleter(m_instance.get());
		m_surface = vk::UniqueSurfaceKHR(surface, _deleter);
	}
	m_physicalDevice = getPhysicalDevice(m_instance, m_surface);
	m_device = getUniqueLogicalDevice(m_physicalDevice, m_surface);
	m_indices = findGraphicAndPresentQueueFamilyIndices(m_physicalDevice, m_surface);

	m_graphicQueue = m_device->getQueue(m_indices.graphicsFamily.value(), 0);
	m_presentQueue = m_device->getQueue(m_indices.presentFamily.value(), 0);

	return *this;
}
