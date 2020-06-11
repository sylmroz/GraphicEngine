#pragma once

#include "../../Common/TextureFactory.hpp"
#include "../../Common/TextureReader.hpp"
#include "VulkanTexture.hpp"

namespace GraphicEngine::Vulkan
{
	using TextureFactory = Common::TextureFactory<Common::TextureReader, Texture2D, vk::PhysicalDevice&, vk::UniqueDevice&, vk::UniqueCommandPool&, vk::Queue&>;
}