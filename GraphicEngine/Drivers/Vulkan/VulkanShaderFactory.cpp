#include "VulkanShaderFactory.hpp"
#include "VulkanFramework.hpp"

GraphicEngine::Vulkan::VulkanShaderFactory::VulkanShaderFactory(VulkanFramework* framework):
	m_framework{framework}
{
}

std::shared_ptr<GraphicEngine::Vulkan::VulkanShader> GraphicEngine::Vulkan::VulkanShaderFactory::getVulkanShader(vk::ShaderStageFlagBits shaderType, const std::string& data)
{
	return std::make_shared<VulkanShader>(m_framework->m_device, data, shaderType);
}

std::shared_ptr<GraphicEngine::Vulkan::VulkanShader> GraphicEngine::Vulkan::VulkanShaderFactory::getVulkanShader(ShaderType shaderType, const std::string& data)
{
	return std::make_shared<VulkanShader>(m_framework->m_device, data, GetVulkanShaderType(shaderType));
}

std::shared_ptr<GraphicEngine::Vulkan::VulkanVertexShader> GraphicEngine::Vulkan::VulkanShaderFactory::getVulkanVertexShader(const std::string& data)
{
	return std::make_shared<VulkanVertexShader>(m_framework->m_device, data);
}

std::shared_ptr<GraphicEngine::Vulkan::VulkanFragmentShader> GraphicEngine::Vulkan::VulkanShaderFactory::getVulkanFragmentShader(const std::string& data)
{
	return std::make_shared<VulkanFragmentShader>(m_framework->m_device, data);
}

std::shared_ptr<GraphicEngine::Vulkan::VulkanGeometryShader> GraphicEngine::Vulkan::VulkanShaderFactory::getVulkanGeometryShader(const std::string& data)
{
	return std::make_shared<VulkanGeometryShader>(m_framework->m_device, data);
}

std::shared_ptr<GraphicEngine::Vulkan::VulkanTessellationControlShader> GraphicEngine::Vulkan::VulkanShaderFactory::getVulkanTessellationControlShader(const std::string& data)
{
	return std::make_shared<VulkanTessellationControlShader>(m_framework->m_device, data);
}

std::shared_ptr<GraphicEngine::Vulkan::VulkanTessellationEvaluationShader> GraphicEngine::Vulkan::VulkanShaderFactory::getVulkanTessellationEvaluationShader(const std::string& data)
{
	return std::make_shared<VulkanTessellationEvaluationShader>(m_framework->m_device, data);
}
