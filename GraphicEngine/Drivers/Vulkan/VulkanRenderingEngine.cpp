#include "VulkanRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"
#include "../../Core/IO/FileSystem.hpp"

#include "VulkanTextureFactory.hpp"
#include "VulkanVertexBufferFactory.hpp"
#include "VulkanUniformBuffer.hpp"

#undef max

GraphicEngine::Vulkan::VulkanRenderingEngine::VulkanRenderingEngine(std::shared_ptr<VulkanWindowContext> vulkanWindowContext,
	std::shared_ptr<Common::Camera> camera,
	std::shared_ptr<Core::EventManager> eventManager,
	std::unique_ptr<Core::Logger<VulkanRenderingEngine>> logger) :
	m_vulkanWindowContext(vulkanWindowContext),
	RenderingEngine(camera, eventManager)
{
}

bool GraphicEngine::Vulkan::VulkanRenderingEngine::drawFrame()
{
	try
	{
		//m_device->waitForFences(1, &(m_renderingBarriers->inFlightFences[m_currentFrameIndex].get()), true, std::numeric_limits<uint64_t>::max());
		//vk::ResultValue<uint32_t> imageIndex = m_device->acquireNextImageKHR(m_swapChainData.swapChain.get(), std::numeric_limits<uint64_t>::max(), m_renderingBarriers->imageAvailableSemaphores[m_currentFrameIndex].get(), vk::Fence());

		//if (imageIndex.result == vk::Result::eErrorOutOfDateKHR)
		//{
		//	// Should recreate swapchain
		//	return false;
		//}

		//else if (imageIndex.result != vk::Result::eSuccess && imageIndex.result != vk::Result::eSuboptimalKHR)
		//{
		//	throw std::runtime_error("Failed to acquire next image!");
		//}

		//if (m_renderingBarriers->imagesInFlight[imageIndex.value] != vk::Fence())
		//	m_device->waitForFences(1, &(m_renderingBarriers->imagesInFlight[imageIndex.value]), true, std::numeric_limits<uint64_t>::max());

		//m_renderingBarriers->imagesInFlight[imageIndex.value] = m_renderingBarriers->inFlightFences[m_currentFrameIndex].get();

		//m_device->resetFences(1, &(m_renderingBarriers->inFlightFences[m_currentFrameIndex].get()));

		m_framework->acquireFrame();

		m_uniformBuffer->updateAndSet(m_camera->getViewProjectionMatrix());
		light.eyePosition = m_camera->getPosition();
		m_lightUniformBuffer->updateAndSet(light);

		m_framework->submitFrame();

		/*m_uniformBuffer->updateAndSet(m_device, m_camera->getViewProjectionMatrix(), imageIndex.value);
		light.eyePosition = m_camera->getPosition();
		m_lightUniformBuffer->updateAndSet(m_device, light, imageIndex.value);*/

		//vk::Semaphore waitSemaphore(m_renderingBarriers->imageAvailableSemaphores[m_currentFrameIndex].get());
		//vk::Semaphore signalSemaphore(m_renderingBarriers->renderFinishedSemaphores[m_currentFrameIndex].get());
		//vk::PipelineStageFlags pipelineStageFlags(vk::PipelineStageFlagBits::eColorAttachmentOutput);
		//vk::SubmitInfo submitInfo(1, &waitSemaphore, &pipelineStageFlags, 1, &(m_commandBuffers[imageIndex.value].get()), 1, &signalSemaphore);
		//vk::Result submitResult = m_graphicQueue.submit(1, &submitInfo, m_renderingBarriers->inFlightFences[m_currentFrameIndex].get());
		//if (submitResult != vk::Result::eSuccess)
		//{
		//	throw std::runtime_error("Failed to submit draw command buffer!");
		//}

		//vk::SwapchainKHR sp(m_swapChainData.swapChain.get());

		//vk::PresentInfoKHR presentInfo(1, &signalSemaphore, 1, &sp, &imageIndex.value);

		//vk::Result presentResult = m_presentQueue.presentKHR(presentInfo);

		//if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR)
		//{
		//	// Should recreate swapchain
		//	return false;
		//}
		//else if (imageIndex.result != vk::Result::eSuccess)
		//{
		//	throw std::runtime_error("failed to present swap chain image!");
		//}
	}

	catch (vk::OutOfDateKHRError err)
	{
		// Should recreate swapchain
		return false;
	}

	//m_currentFrameIndex = calculateNextIndex();
	return true;
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::init(size_t width, size_t height)
{
	try
	{
		//m_vertexBuffer = m_mesh->compile<VertexBufferFactory<Common::VertexPCTc>, VertexBuffer<Common::VertexPCTc>>(m_physicalDevice, m_device, m_commandPool, m_graphicQueue);

		/*m_vertexShader = std::make_unique<VulkanShader>(m_device, Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("basicPCTVP.vert.spv").string()));
		m_fragmentShader = std::make_unique<VulkanShader>(m_device, Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("basicPCTVP.frag.spv").string()));*/

		//m_texture = TextureFactory::produceTexture("C:/rem.png", m_physicalDevice, m_device, m_commandPool, m_graphicQueue);
		/*m_descriptorSetLayout = createDescriptorSetLayout(m_device, 
			{ {vk::DescriptorType::eUniformBuffer,1,vk::ShaderStageFlagBits::eVertex}, {vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment} },
			vk::DescriptorSetLayoutCreateFlags());
		m_descriptorPool = createDescriptorPool(m_device, 
			{ vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, m_maxFrames),
			vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, m_maxFrames)});*/

		//uniformBuffers.emplace_back(m_uniformBuffer->bufferData);
		
		/*std::vector<std::shared_ptr<Texture2D>> textures;
		textures.emplace_back(m_texture);*/
		//updateDescriptorSets(m_device, m_descriptorPool, m_descriptorSetLayout, m_maxFrames, m_descriptorSets, uniformBuffers, textures);

		//m_pipelineLayout = m_device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &m_descriptorSetLayout.get()));

		//m_pipelineCache = m_device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
		///*m_graphicPipeline = createGraphicPipeline(m_device, m_pipelineCache, ShaderInfo{ m_vertexShader->shaderModule.get(),vk::SpecializationInfo() },
		//	ShaderInfo{ m_fragmentShader->shaderModule.get(),vk::SpecializationInfo() }, createVertexInputAttributeDescriptions(Common::VertexPCTc::getSizeAndOffsets()),
		//	vk::VertexInputBindingDescription(0, Common::VertexPCTc::getStride()), true, vk::FrontFace::eClockwise, m_pipelineLayout, m_renderPass, m_msaaSamples);*/

		m_framework = std::make_unique<VulkanFramework>();
		m_framework->
			initialize(m_vulkanWindowContext, "Graphic Engine", "Vulkan Base", width, height, vk::SampleCountFlagBits::e2, { "VK_LAYER_KHRONOS_validation" })
			.initializeCommandBuffer()
			.initializeFramebuffer()
			.initalizeRenderingBarriers();

		m_uniformBuffer = m_framework->getUniformBuffer<glm::mat4>();
		m_lightUniformBuffer = m_framework->getUniformBuffer<Light>();

		m_vertexBuffers = m_model->compile<VertexBufferFactory, VertexBuffer>(m_framework->m_physicalDevice, m_framework->m_device, m_framework->m_commandPool, m_framework->m_graphicQueue);
		m_vertexShader = std::make_unique<VulkanShader>(m_framework->m_device, Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("diffuse.vert.spv").string()));
		m_fragmentShader = std::make_unique<VulkanShader>(m_framework->m_device, Core::IO::readFile<std::string>(Core::FileSystem::getVulkanShaderPath("diffuse.frag.spv").string()));

		m_descriptorSetLayout = createDescriptorSetLayout(m_framework->m_device,
			{ {vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex},
			{vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eFragment} },
			vk::DescriptorSetLayoutCreateFlags());

		m_descriptorPool = createDescriptorPool(m_framework->m_device,
			{ vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, m_framework->m_maxFrames),
			vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, m_framework->m_maxFrames) });

		std::vector<vk::DescriptorSetLayout> layouts(m_framework->m_maxFrames, m_descriptorSetLayout.get());

		m_descriptorSets = m_framework->m_device->allocateDescriptorSetsUnique(vk::DescriptorSetAllocateInfo(m_descriptorPool.get(), layouts.size(), layouts.data()));

		std::vector<std::vector<std::shared_ptr<BufferData>>> uniformBuffers{ {m_uniformBuffer->bufferData, m_lightUniformBuffer->bufferData} };

		updateDescriptorSets(m_framework->m_device, m_descriptorPool, m_descriptorSetLayout, m_framework->m_maxFrames, m_descriptorSets, uniformBuffers, {});

		m_pipelineLayout = m_framework->m_device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &m_descriptorSetLayout.get()));

		m_pipelineCache = m_framework->m_device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());

		m_graphicPipeline = createGraphicPipeline(m_framework->m_device, m_pipelineCache, ShaderInfo{ m_vertexShader->shaderModule.get(),vk::SpecializationInfo() },
			ShaderInfo{ m_fragmentShader->shaderModule.get(),vk::SpecializationInfo() }, createVertexInputAttributeDescriptions(Common::VertexPN::getSizeAndOffsets()),
			vk::VertexInputBindingDescription(0, Common::VertexPN::getStride()), true, vk::FrontFace::eClockwise, m_pipelineLayout, m_framework->m_renderPass, m_framework->m_msaaSamples, vk::CullModeFlagBits::eNone);

		buildCommandBuffers();
	}

	catch (vk::SystemError& err)
	{
		throw std::runtime_error(err.what());
	}

	catch (std::runtime_error& err)
	{
		throw std::runtime_error(err.what());
	}

	catch (...)
	{
		throw std::runtime_error("Vulkan Initialize: unknown error\n");
	}
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::resizeFrameBuffer(size_t width, size_t height)
{
	if (width == 0 || height == 0)
		return;

	m_framework->initializeFramebuffer(width, height);

	buildCommandBuffers();
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::cleanup()
{
	m_framework->m_graphicQueue.waitIdle();
	m_framework->m_device->waitIdle();
}

void GraphicEngine::Vulkan::VulkanRenderingEngine::buildCommandBuffers()
{
	std::array<vk::ClearValue, 3> clearValues;

	clearValues[0].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f,0.2f,0.2f,1.0f }));
	clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0.0f);
	clearValues[2].color = vk::ClearColorValue(std::array<float, 4>({ 0.2f,0.2f,0.2f,1.0f }));
	int i{ 0 };
	for (auto& commandBuffer : m_framework->m_commandBuffers)
	{
		commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlags()));

		commandBuffer->setViewport(0, vk::Viewport(0.0f, static_cast<float>(m_framework->m_swapChainData.extent.height),
			static_cast<float>(m_framework->m_swapChainData.extent.width), -static_cast<float>(m_framework->m_swapChainData.extent.height), 0.0f, 1.0f));
		commandBuffer->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_framework->m_swapChainData.extent));

		vk::RenderPassBeginInfo renderPassBeginInfo(m_framework->m_renderPass.get(), m_framework->m_frameBuffers[i].get(), vk::Rect2D(vk::Offset2D(0, 0), m_framework->m_swapChainData.extent), static_cast<uint32_t>(clearValues.size()), clearValues.data());
		commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

		commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicPipeline.get());

		commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout.get(), 0, 1, &m_descriptorSets[i].get(), 0, nullptr);
		/*m_vertexBuffer->bind(commandBuffer);
		m_vertexBuffer->draw(commandBuffer);*/
		for (auto& vb : m_vertexBuffers)
		{
			vb->bind(commandBuffer);
			vb->draw(commandBuffer);
		}

		commandBuffer->endRenderPass();

		commandBuffer->end();
		++i;
	}
}
