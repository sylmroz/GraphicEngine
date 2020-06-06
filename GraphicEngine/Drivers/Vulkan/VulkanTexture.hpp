#pragma once

#include "VulkanHelper.hpp"

namespace GraphicEngine::Vulkan
{
	class Texture2D : public ImageData
	{
	public:
		Texture2D(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::UniqueCommandPool& commandPool, vk::Queue queue,
			vk::Format format, int mipLevel, int width, int height, int channels, const uint8_t* data);

		vk::UniqueSampler sampler;

		virtual ~Texture2D() = default;
	};
}

