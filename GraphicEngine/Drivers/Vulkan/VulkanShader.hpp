#ifndef GRAPHIC_ENGINE_DRIVERS_VULKAN_SHADER_VULKAN_HPP
#define GRAPHIC_ENGINE_DRIVERS_VULKAN_SHADER_VULKAN_HPP

#include "../../Common/Shader.hpp"

#include <vulkan/vulkan.hpp>

namespace GraphicEngine::Vulkan
{
	ShaderType GetShaderType(vk::ShaderStageFlags shaderType);
	vk::ShaderStageFlagBits GetVulkanShaderType(ShaderType shaderType);
	
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const vk::UniqueDevice& device, const std::string& code, vk::ShaderStageFlagBits shaderType) :
			Shader{ code, GetShaderType(shaderType) }, m_shaderType{ shaderType }
		{
			m_device = device.get();
			compile();
		}

		template <typename Reader>
		VulkanShader(const vk::UniqueDevice& device, Reader reader, const std::string& path, vk::ShaderStageFlagBits shaderType) :
			Shader{ reader, path, GetShaderType(shaderType) }, m_shaderType{ shaderType }
		{
			m_device = device.get();
			compile();
		}

		vk::ShaderStageFlagBits getVulkanShaderType()
		{
			return m_shaderType;
		}

	public:
		vk::UniqueShaderModule shaderModule;

	protected:
		virtual void compile() override
		{
			shaderModule = m_device.createShaderModuleUnique(vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(),
				static_cast<uint32_t>(m_data.size()), reinterpret_cast<uint32_t*>(m_data.data())));
		}

	protected:
		vk::Device m_device;
		vk::ShaderStageFlagBits m_shaderType;
	};

	class VulkanVertexShader : public VulkanShader
	{
	public:
		VulkanVertexShader(const vk::UniqueDevice& device, const std::string& code) :
			VulkanShader{ device, code, vk::ShaderStageFlagBits::eVertex } 
		{}

		template <typename Reader>
		VulkanVertexShader(const vk::UniqueDevice& device, Reader reader, const std::string& path):
			VulkanShader{ device, reader, path, vk::ShaderStageFlagBits::eVertex }
		{}
	};

	class VulkanFragmentShader : public VulkanShader
	{
	public:
		VulkanFragmentShader(const vk::UniqueDevice& device, const std::string& code) :
			VulkanShader{ device, code, vk::ShaderStageFlagBits::eFragment }
		{}

		template <typename Reader>
		VulkanFragmentShader(const vk::UniqueDevice& device, Reader reader, const std::string& path) :
			VulkanShader{ device, reader, path, vk::ShaderStageFlagBits::eFragment }
		{}
	};

	class VulkanGeometryShader : public VulkanShader
	{
	public:
		VulkanGeometryShader(const vk::UniqueDevice& device, const std::string& code) :
			VulkanShader{ device, code, vk::ShaderStageFlagBits::eGeometry }
		{}

		template <typename Reader>
		VulkanGeometryShader(const vk::UniqueDevice& device, Reader reader, const std::string& path) :
			VulkanShader{ device, reader, path, vk::ShaderStageFlagBits::eGeometry }
		{}
	};

	class VulkanTessellationControlShader : public VulkanShader
	{
	public:
		VulkanTessellationControlShader(const vk::UniqueDevice& device, const std::string& code) :
			VulkanShader{ device, code, vk::ShaderStageFlagBits::eTessellationControl }
		{}

		template <typename Reader>
		VulkanTessellationControlShader(const vk::UniqueDevice& device, Reader reader, const std::string& path) :
			VulkanShader{ device, reader, path, vk::ShaderStageFlagBits::eTessellationControl }
		{}
	};

	class VulkanTessellationEvaluationShader : public VulkanShader
	{
	public:
		VulkanTessellationEvaluationShader(const vk::UniqueDevice& device, const std::string& code) :
			VulkanShader{ device, code, vk::ShaderStageFlagBits::eTessellationEvaluation }
		{}

		template <typename Reader>
		VulkanTessellationEvaluationShader(const vk::UniqueDevice& device, Reader reader, const std::string& path) :
			VulkanShader{ device, reader, path, vk::ShaderStageFlagBits::eTessellationEvaluation }
		{}
	};
}

#endif // !GRAPHIC_ENGINE_DRIVERS_VULKAN_SHADER_VULKAN_HPP
