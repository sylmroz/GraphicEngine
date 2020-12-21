#include "ImGuiImpl.hpp"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>

void GraphicEngine::GUI::ImGuiImpl::drawUi()
{
	ImGui::Begin("Demo window");
	static glm::vec4 color;
	ImGui::ColorEdit3("color", &color[0]);
	ImGui::End();

	ImGui::Render();
}

bool GraphicEngine::GUI::ImGuiImpl::isKeyboardBusy()
{
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureKeyboard;
}

bool GraphicEngine::GUI::ImGuiImpl::isMouseButtonBusy()
{
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureMouse;
}

GraphicEngine::Common::UI& GraphicEngine::GUI::ImGuiImpl::initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	io.ConfigMemoryCompactTimer = -1.0f;
	return UI::initialize();
}

void GraphicEngine::GUI::ImGuiImpl::nextFrame()
{
	UI::nextFrame();
	ImGui::NewFrame();
}

void GraphicEngine::GUI::ImGuiImpl::shutdown()
{
	UI::shutdown();
	ImGui::DestroyContext();
}

GraphicEngine::GUI::ImGuiImpl::GlfwEngineBackend::GlfwEngineBackend(std::shared_ptr<GLFWwindow> window) :
	window{ window }
{
}

void GraphicEngine::GUI::ImGuiImpl::GlfwEngineBackend::nextFrame()
{
	ImGui_ImplGlfw_NewFrame();
}

void GraphicEngine::GUI::ImGuiImpl::GlfwEngineBackend::shutdown()
{
	ImGui_ImplGlfw_Shutdown();
}

GraphicEngine::GUI::ImGuiImpl::GlfwOpenGlEngineBackend::GlfwOpenGlEngineBackend(std::shared_ptr<GLFWwindow> window) :
	GlfwEngineBackend{ window }
{
}

void GraphicEngine::GUI::ImGuiImpl::GlfwOpenGlEngineBackend::initialize()
{
	ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
}

void GraphicEngine::GUI::ImGuiImpl::OpenGlRenderEngineBackend::initialize()
{
	ImGui_ImplOpenGL3_Init("#version 150");
}

void GraphicEngine::GUI::ImGuiImpl::OpenGlRenderEngineBackend::nextFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
}

void GraphicEngine::GUI::ImGuiImpl::OpenGlRenderEngineBackend::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
}

void GraphicEngine::GUI::ImGuiImpl::OpenGlRenderEngineBackend::renderData()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GraphicEngine::GUI::ImGuiImpl::GlfwVulkanEngineBackend::GlfwVulkanEngineBackend(std::shared_ptr<GLFWwindow> window) :
	GlfwEngineBackend{ window }
{
}

void GraphicEngine::GUI::ImGuiImpl::GlfwVulkanEngineBackend::initialize()
{
	ImGui_ImplGlfw_InitForVulkan(window.get(), true);
}

GraphicEngine::GUI::ImGuiImpl::VulkanRenderEngineBackend::VulkanRenderEngineBackend(std::shared_ptr<Vulkan::VulkanFramework> framework) :
	m_framework{ framework }
{
}

void GraphicEngine::GUI::ImGuiImpl::VulkanRenderEngineBackend::initialize()
{
	// TODO: Create specific render pass for UI
	m_pipelineCache = m_framework->m_device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());

	m_descriptorPool = Vulkan::createDescriptorPool(m_framework->m_device,
		{
			vk::DescriptorPoolSize(vk::DescriptorType::eSampler, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eSampledImage, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eUniformTexelBuffer, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eStorageTexelBuffer, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eUniformBufferDynamic, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eStorageBufferDynamic, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eInputAttachment, 1000)
		});

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = m_framework->m_instance.get();
	init_info.PhysicalDevice = m_framework->m_physicalDevice;
	init_info.Device = m_framework->m_device.get();
	init_info.QueueFamily = m_framework->m_indices.graphicsFamily.value();
	init_info.Queue = m_framework->m_graphicQueue;
	init_info.PipelineCache = m_pipelineCache.get();
	init_info.DescriptorPool = m_descriptorPool.get();
	init_info.Allocator = nullptr;
	init_info.MinImageCount = m_framework->m_maxFrames;
	init_info.ImageCount = m_framework->m_maxFrames;
	init_info.MSAASamples = (VkSampleCountFlagBits)m_framework->m_msaaSamples;
	init_info.CheckVkResultFn = [](VkResult err)
	{
		//std::cout << err << std::endl;
	};
	ImGui_ImplVulkan_Init(&init_info, m_framework->m_renderPass.get());

	Vulkan::singleTimeCommand(m_framework->m_device, m_framework->m_commandPool, m_framework->m_graphicQueue, [&](const vk::UniqueCommandBuffer& commandBuffer)
	{
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer.get());
	});
}

void GraphicEngine::GUI::ImGuiImpl::VulkanRenderEngineBackend::nextFrame()
{
	ImGui_ImplVulkan_NewFrame();
}

void GraphicEngine::GUI::ImGuiImpl::VulkanRenderEngineBackend::shutdown()
{
	ImGui_ImplVulkan_Shutdown();
}

void GraphicEngine::GUI::ImGuiImpl::VulkanRenderEngineBackend::renderData(vk::UniqueCommandBuffer& commandBuffer)
{
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer.get());
}
