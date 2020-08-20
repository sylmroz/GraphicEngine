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
};
