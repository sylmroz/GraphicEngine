#ifndef GRAPHIC_ENGINE_DRIVERS_VULKAN_SHADER_VULKAN_HPP
#define GRAPHIC_ENGINE_DRIVERS_VULKAN_SHADER_VULKAN_HPP

#include "../../Core/Shader.hpp"

#include <vulkan/vulkan.hpp>

class ShaderVulkan : public Shader<ShaderVulkan>
{
public:
	ShaderVulkan(const vk::UniqueDevice& device, const std::string& path) :
		Shader<ShaderVulkan>(path)
	{
		_device = device.get();
		compile();
	}

	vk::UniqueShaderModule shaderModule;

public:
	void _compile()
	{
		shaderModule = _device.createShaderModuleUnique(vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), static_cast<uint32_t>(_data.size()), reinterpret_cast<uint32_t*>(_data.data())));
	}
protected:
	vk::Device _device;
};

#endif // !GRAPHIC_ENGINE_DRIVERS_VULKAN_SHADER_VULKAN_HPP
