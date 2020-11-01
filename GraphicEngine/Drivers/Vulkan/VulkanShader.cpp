#include "VulkanShader.hpp"
#include <map>

GraphicEngine::ShaderType GraphicEngine::Vulkan::GetShaderType(vk::ShaderStageFlags shaderType)
{
	std::map<vk::ShaderStageFlags, ShaderType> shaderTypeMap =
	{
		{ vk::ShaderStageFlagBits::eVertex, ShaderType::Vertex },
		{ vk::ShaderStageFlagBits::eFragment, ShaderType::Fragment },
		{ vk::ShaderStageFlagBits::eGeometry, ShaderType::Geometry },
		{ vk::ShaderStageFlagBits::eTessellationControl, ShaderType::TessalationControll },
		{ vk::ShaderStageFlagBits::eTessellationEvaluation, ShaderType::TessalationEvaluation }
	};

	return shaderTypeMap[shaderType];
}
vk::ShaderStageFlagBits GraphicEngine::Vulkan::GetVulkanShaderType(ShaderType shaderType)
{
	std::map<ShaderType, vk::ShaderStageFlagBits> shaderTypeMap =
	{
		{ ShaderType::Vertex, vk::ShaderStageFlagBits::eVertex },
		{ ShaderType::Fragment , vk::ShaderStageFlagBits::eFragment },
		{ ShaderType::Geometry, vk::ShaderStageFlagBits::eGeometry },
		{ ShaderType::TessalationControll, vk::ShaderStageFlagBits::eTessellationControl },
		{ ShaderType::TessalationEvaluation, vk::ShaderStageFlagBits::eTessellationEvaluation }
	};

	return shaderTypeMap[shaderType];
}
