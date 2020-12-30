#pragma once

#include "VulkanHelper.hpp"

namespace GraphicEngine::Vulkan
{
	class TextureCube : public Texture
	{
	public:
		TextureCube(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::UniqueCommandPool& commandPool, vk::Queue queue,
			const uint8_t* data, int width, int height, int channels);

		TextureCube(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::array<std::string, 6>& faces);

		void initialize(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::UniqueCommandPool& commandPool, vk::Queue queue,
			const uint8_t* data, int width, int height, int channels);

		virtual ~TextureCube() = default;

		static auto getFormat(int channels)
		{
			return internalFormats.at(static_cast<size_t>(channels) - 1);
		}

	private:
		static constexpr std::array<vk::Format, 4> internalFormats = { {vk::Format::eR8Srgb, vk::Format::eR8G8Srgb, vk::Format::eR8G8B8Srgb, vk::Format::eR8G8B8A8Srgb} };
	};
}
