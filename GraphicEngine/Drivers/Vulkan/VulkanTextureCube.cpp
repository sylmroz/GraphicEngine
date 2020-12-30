#include "VulkanTextureCube.hpp"

#include "../../Common/TextureReader.hpp"
#include "../../Core/Math/ImageUtils.hpp"

GraphicEngine::Vulkan::TextureCube::TextureCube(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::UniqueCommandPool& commandPool, vk::Queue queue, 
	const uint8_t* data, int width, int height, int channels) :
	Texture(physicalDevice, device, vk::Extent3D(width, height, 1), getFormat(channels), vk::SampleCountFlagBits::e1, vk::MemoryPropertyFlagBits::eDeviceLocal,
		vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
		vk::ImageTiling::eOptimal, Core::calculateMipLevels(width, height), 6, vk::ImageLayout::eUndefined, vk::ImageAspectFlagBits::eColor, vk::ImageType::e2D, vk::ImageViewType::eCube, vk::ImageCreateFlagBits::eCubeCompatible)
{
	initialize(physicalDevice, device, commandPool, queue, data, width, height, channels);
}

GraphicEngine::Vulkan::TextureCube::TextureCube(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::UniqueCommandPool& commandPool, vk::Queue queue, const std::array<std::string, 6>& faces)
{
	int width{ 0 };
	int height{ 0 };
	int channels{ 0 };
	std::vector<uint8_t> data;
	data.reserve(width * height * channels * 6);
	for (const auto& path : faces)
	{
		Common::TextureReader textureReader(path);
		auto [pixels, w, h, c] = textureReader();
		width = w;
		height = h;
		channels = c;
		int size = width * height * channels;
		data.insert(data.end(), pixels, pixels + size);
	}
	initImageData(physicalDevice, device, vk::Extent3D(width, height, 1), vk::Format::eR8G8B8A8Unorm, vk::SampleCountFlagBits::e1, vk::MemoryPropertyFlagBits::eDeviceLocal,
		vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
		vk::ImageTiling::eOptimal, /*Core::calculateMipLevels(width, height)*/ 1, 6, vk::ImageLayout::eUndefined, vk::ImageAspectFlagBits::eColor, vk::ImageType::e2D, vk::ImageViewType::eCube, vk::ImageCreateFlagBits::eCubeCompatible);
	initialize(physicalDevice, device, commandPool, queue, data.data(), width, height, channels);
}

void GraphicEngine::Vulkan::TextureCube::initialize(const vk::PhysicalDevice& physicalDevice, const vk::UniqueDevice& device, vk::UniqueCommandPool& commandPool, vk::Queue queue, const uint8_t* data, int width, int height, int channels)
{
	int size = width * height * channels * 6;
	//int mipLevels = Core::calculateMipLevels(width, height);

	BufferData stagingBuffer(physicalDevice, device, vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, sizeof(uint8_t) * size);
	copyMemoryToDevice<uint8_t>(device, stagingBuffer.memory, data, size);
	transitionImageLayout(device, commandPool, queue, image, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, /*mipLevels*/1, 6);
	singleTimeCommand(device, commandPool, queue, [&](const vk::UniqueCommandBuffer& commandBuffer)
	{
		std::vector<vk::BufferImageCopy> regions;
		uint32_t offset{ 0 };
		for (int face{ 0 }; face < 6; ++face)
		{
			vk::ImageSubresourceLayers imageSubresource(vk::ImageAspectFlagBits::eColor, 0, face, 1);
			vk::BufferImageCopy region(offset, 0, 0, imageSubresource, { 0,0,0 }, { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 });
			offset += (width * height * channels);
			regions.push_back(region);
		}
		commandBuffer->copyBufferToImage(stagingBuffer.buffer.get(), image.get(), vk::ImageLayout::eTransferDstOptimal, regions.size(), regions.data());
	});

	transitionImageLayout(device, commandPool, queue, image, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, /*mipLevels*/1, 6);

	//generateMipmaps(physicalDevice, device, commandPool, queue, image, getFormat(channels), width, height, mipLevels);

	vk::SamplerCreateInfo samplerCreateInfo({}, vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear,
		vk::SamplerAddressMode::eClampToEdge, vk::SamplerAddressMode::eClampToEdge, vk::SamplerAddressMode::eClampToEdge,
		0.0f, true, 16.0f, false, vk::CompareOp::eNever, 0.0f, static_cast<float>(/*mipLevels*/1), vk::BorderColor::eIntOpaqueWhite, false);
	sampler = device->createSamplerUnique(samplerCreateInfo);
}
