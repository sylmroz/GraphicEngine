#pragma once

#include "VulkanShader.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanFramework;
	class VulkanShaderFactory
	{
	public:
		VulkanShaderFactory(VulkanFramework* framework);

		std::shared_ptr<VulkanShader> getVulkanShader(vk::ShaderStageFlagBits shaderType, const std::string& data);

		std::shared_ptr<VulkanShader> getVulkanShader(ShaderType shaderType, const std::string& data);

		std::shared_ptr<VulkanVertexShader> getVulkanVertexShader(const std::string& data);

		std::shared_ptr<VulkanFragmentShader> getVulkanFragmentShader(const std::string& data);

		std::shared_ptr<VulkanGeometryShader> getVulkanGeometryShader(const std::string& data);

		std::shared_ptr<VulkanTessellationControlShader> getVulkanTessellationControlShader(const std::string& data);

		std::shared_ptr<VulkanTessellationEvaluationShader> getVulkanTessellationEvaluationShader(const std::string& data);

	protected:
		VulkanFramework* m_framework;
	};
}