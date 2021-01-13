#pragma once

#include "../../Common/UI.hpp"
#include "../../Drivers/Vulkan/VulkanFramework.hpp"
#include "Components/SettingsWindow.hpp"

struct GLFWwindow;

namespace GraphicEngine::GUI
{
	class ImGuiImpl : public Common::UI
	{
	public:
		struct GlfwEngineBackend : EngineBackend
		{
			GlfwEngineBackend(std::shared_ptr<GLFWwindow> window);
			virtual void nextFrame() override;
			virtual void shutdown() override;
			virtual ~GlfwEngineBackend() = default;

		protected:
			std::shared_ptr<GLFWwindow> window;
		};

		struct GlfwOpenGlEngineBackend : GlfwEngineBackend
		{
			GlfwOpenGlEngineBackend(std::shared_ptr<GLFWwindow> window);
			virtual void initialize() override;
			virtual ~GlfwOpenGlEngineBackend() = default;
		};

		struct OpenGlRenderEngineBackend : RenderEngineBackend<OpenGlRenderEngineBackend>
		{
			virtual void initialize() override;
			virtual void nextFrame() override;
			virtual void shutdown() override;
			void renderData();
		};

		struct GlfwVulkanEngineBackend : GlfwEngineBackend
		{
			GlfwVulkanEngineBackend(std::shared_ptr<GLFWwindow> window);
			virtual void initialize() override;
			virtual ~GlfwVulkanEngineBackend() = default;
		};

		struct VulkanRenderEngineBackend : RenderEngineBackend<VulkanRenderEngineBackend, vk::UniqueCommandBuffer&>
		{
			VulkanRenderEngineBackend(std::shared_ptr<Vulkan::VulkanFramework> framework);
			virtual void initialize() override;
			virtual void nextFrame() override;
			virtual void shutdown() override;
			void renderData(vk::UniqueCommandBuffer& commandBuffer);

		private:
			vk::UniqueDescriptorPool m_descriptorPool;
			vk::UniquePipelineCache m_pipelineCache;
			std::shared_ptr<Vulkan::VulkanFramework> m_framework;
		};

	public:
		ImGuiImpl(std::shared_ptr<SettingWindow> settingWindow);
		virtual void drawUi() override;
		virtual bool isKeyboardBusy() override;
		virtual bool isMouseButtonBusy() override;
		virtual UI& initialize();
		virtual void nextFrame();
		virtual void shutdown();
	};
}