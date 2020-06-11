#include "VulkanTexture.hpp"

GraphicEngine::Vulkan::Texture2D::Texture2D(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::UniqueCommandPool& commandPool, vk::Queue queue,
	int mipLevel, int width, int height, int channels, const uint8_t* data) :
	ImageData(physicalDevice, device, vk::Extent3D(width, height, 1), getFormat(channels), vk::SampleCountFlagBits::e1, vk::MemoryPropertyFlagBits::eDeviceLocal,
		vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
		vk::ImageTiling::eOptimal, mipLevel, vk::ImageLayout::eUndefined, vk::ImageAspectFlagBits::eColor, vk::ImageType::e2D)
{
	int size = width * height * channels;
	BufferData stagingBuffer(physicalDevice, device, vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, sizeof(uint8_t) * size);
	copyMemoryToDevice<uint8_t>(device, stagingBuffer.memory, data, size);
	transitionImageLayout(device, commandPool, queue, image, getFormat(channels), vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, mipLevel);
	singleTimeCommand(device, commandPool, queue, [&](const vk::UniqueCommandBuffer& commandBuffer)
		{
			vk::ImageSubresourceLayers imageSubresource(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
			vk::BufferImageCopy region(0, 0, 0, imageSubresource, { 0,0,0 }, { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 });
			commandBuffer->copyBufferToImage(stagingBuffer.buffer.get(), image.get(), vk::ImageLayout::eTransferDstOptimal, 1, &region);
		});

	generateMipmaps(physicalDevice, device, commandPool, queue, image, getFormat(channels), width, height, mipLevel);

	vk::SamplerCreateInfo samplerCreateInfo({}, vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear,
		vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat,
		0.0f, true, 16.0f, false, vk::CompareOp::eAlways, 0.0f, static_cast<float>(mipLevel), vk::BorderColor::eIntOpaqueBlack, false);
	sampler = device->createSamplerUnique(samplerCreateInfo);

}
