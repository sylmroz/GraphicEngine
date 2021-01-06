#pragma once

#include "../../Common/RenderingEngine.hpp"
#include "VulkanShader.hpp"
#include "VulkanFramework.hpp"
#include "VulkanWindowContext.hpp"
#include "VulkanTexture.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanUniformBuffer.hpp"

#include "../../Engines/Graphic/Shaders/Models/ModelMatrices.hpp"

#include "Pipelines/VulkanNormalDebugGraphicPileline.hpp"
#include "Pipelines/VulkanWireframeGraphicPipeline.h"
#include "Pipelines/VulkanSolidColorGraphicPipeline.hpp"
#include "Pipelines/VulkanSkyboxGraphicPipeline.hpp"

#include "../../UI/ImGui/ImGuiImpl.hpp"

namespace GraphicEngine::Vulkan
{
	class VulkanRenderingEngine : public RenderingEngine
	{
	public:
		VulkanRenderingEngine(std::shared_ptr<VulkanWindowContext> vulkanWindowContext,
			std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
			std::shared_ptr<Services::ModelManager> modelManager,
			std::shared_ptr<Services::LightManager> lightManager,
			std::shared_ptr<Core::EventManager> eventManager,
			std::shared_ptr<Common::UI> ui,
			std::shared_ptr<Core::Configuration> cfg,
			std::unique_ptr<Core::Logger<VulkanRenderingEngine>> logger);
		virtual bool drawFrame() override;
		virtual void init(size_t width, size_t height) override;
		virtual void resizeFrameBuffer(size_t width, size_t height) override;
		virtual void cleanup() override;

		virtual ~VulkanRenderingEngine() = default;
	private:
		void buildCommandBuffers();
	private:
		std::shared_ptr<VulkanFramework> m_framework;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> m_cameraUniformBuffer;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::Light>> m_lightUniformBuffer;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::Eye>> m_eyePositionUniformBuffer;

	private:
		std::shared_ptr<VulkanWindowContext> m_vulkanWindowContext;

	private:
		std::shared_ptr<VulkanWireframeGraphicPipeline> m_wireframeGraphicPipeline;
		std::shared_ptr<VulkanSolidColorGraphicPipeline> m_solidColorraphicPipeline;
		std::shared_ptr<VulkanNormalDebugGraphicPipeline> m_normalDebugGraphicPipeline;
		std::shared_ptr<VulkanSkyboxGraphicPipeline> m_skyboxGraphicPipeline;

		std::shared_ptr<GUI::ImGuiImpl::VulkanRenderEngineBackend> m_uiRenderingBackend;
	};
}
