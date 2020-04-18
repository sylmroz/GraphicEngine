#ifndef GRAPHIC_ENGINE_DRIVERS_VULKAN_SHADER_VULKAN_HPP
#define GRAPHIC_ENGINE_DRIVERS_VULKAN_SHADER_VULKAN_HPP

#include "../../Common/Shader.hpp"

#include <vulkan/vulkan.hpp>

namespace GraphicEngine::Vulkan
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const vk::UniqueDevice& device, const std::string& code) :
			Shader(code)
		{
			m_device = device.get();
			compile();
		}

		template <typename Reader>
		VulkanShader(const vk::UniqueDevice& device, Reader reader, const std::string& path) :
			Shader(reader, path)
		{
			m_device = device.get();
			compile();
		}

	public:
		vk::UniqueShaderModule shaderModule;

	protected:
		virtual void compile()
		{
			shaderModule = m_device.createShaderModuleUnique(vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), static_cast<uint32_t>(m_data.size()), reinterpret_cast<uint32_t*>(m_data.data())));
		}
	protected:
		vk::Device m_device;
	};
}

#endif // !GRAPHIC_ENGINE_DRIVERS_VULKAN_SHADER_VULKAN_HPP
