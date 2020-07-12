#pragma once

#include "../../Common/VertexBufferFactory.hpp"
#include "VulkanVertexBuffer.hpp"

namespace GraphicEngine::Vulkan
{
	template <typename Vertex>
	using VertexBufferFactory = Common::VertexBufferFactory<VertexBuffer, Vertex, vk::PhysicalDevice&, vk::UniqueDevice&, vk::UniqueCommandPool&, vk::Queue&>;
}