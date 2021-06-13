#pragma once

#include "../../Common/RenderingEngine.hpp"
#include "../../Core/Logger.hpp"
#include "../../Engines/Graphic/Shaders/Models/ModelMatrices.hpp"
#include "../../Engines/Graphic/Shaders/Models/Time.hpp"

#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLUniformBuffer.hpp"
#include "OpenGLVertexBuffer.hpp"
#include "OpenGLShaderStorageBufferObject.hpp"

// Pipelines
#include "GraphicPipelines/OpenGLWireframeGraphicPipeline.hpp"
#include "GraphicPipelines/OpenGLSolidColorGraphicPipeline.hpp"
#include "GraphicPipelines/OpenGLNormalDebugGraphicPileline.hpp"
#include "GraphicPipelines/OpenGLSkyboxGraphicPipeline.hpp"
#include "GraphicPipelines/OpenGLShadowMapGraphicPipeline.hpp"
#include "GraphicPipelines/OpenGLGrassGraphicPipeline.hpp"

#include "../../UI/ImGui/ImGuiImpl.hpp"

namespace GraphicEngine::OpenGL
{
	class OpenGLRenderingEngine : public GraphicEngine::RenderingEngine
	{
	public:
		OpenGLRenderingEngine(std::shared_ptr<Services::ServicesManager> servicesManager,
			std::shared_ptr<Core::EventManager> eventManager,
			std::shared_ptr<Core::Timer> timer,
			std::shared_ptr<Common::UI> ui,
			std::shared_ptr<Core::Configuration> cfg,
			std::unique_ptr<Core::Logger<OpenGLRenderingEngine>> logger);
		virtual bool drawFrame() override;
		virtual void init(size_t width, size_t height) override;
		virtual void resizeFrameBuffer(size_t width, size_t height) override;
		virtual void cleanup() override;

		virtual ~OpenGLRenderingEngine() = default;
	private:
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> m_cameraUniformBuffer;
		std::unique_ptr<UniformBuffer<Engines::Graphic::Shaders::Eye>> m_eyeUniformBuffer;
		std::unique_ptr<UniformBuffer<Engines::Graphic::Shaders::Time>> m_timeUniformBuffer;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::RenderingOptions>> m_renderingOptionsUniformBuffer;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::WindParameters>> m_windParametersUniformBuffer;
		std::shared_ptr<ShaderStorageBufferObject<Engines::Graphic::Shaders::DirectionalLight>> m_directionalLight;
		std::shared_ptr<ShaderStorageBufferObject<Engines::Graphic::Shaders::PointLight>> m_pointLights;
		std::shared_ptr<ShaderStorageBufferObject<Engines::Graphic::Shaders::SpotLight>> m_spotLight;

		std::shared_ptr<Texture> m_directionalLightDepthTexture;
		std::shared_ptr<Texture> m_spotLightdepthTexture;
		std::shared_ptr<Texture> m_pointightdepthTexture;

		std::unique_ptr<Core::Logger<OpenGLRenderingEngine>> m_logger;

		std::unique_ptr<OpenGLWireframeGraphicPipeline> m_wireframeGraphicPipeline;
		std::unique_ptr<OpenGLSolidColorGraphicPipeline> m_solidColorGraphicPipeline;
		std::unique_ptr<OpenGLNormalDebugGraphicPipeline> m_normalDebugGraphicPipeline;
		std::unique_ptr<OpenGLGrassGraphicPipeline> m_grassGraphicPipeline;
		std::unique_ptr<OpenGLSkyboxGraphicPipeline> m_skyboxGraphicPipeline;
		std::unique_ptr<OpenGLShadowMapGraphicPipeline> m_shadowMapGraphicPipeline;
		std::unique_ptr<OpenGLShadowMapGraphicPipeline> m_spotLightshadowMapGraphicPipeline;
		std::unique_ptr<OpenGLShadowMapGraphicPipeline> m_pointLightshadowMapGraphicPipeline;

		std::shared_ptr<GUI::ImGuiImpl::OpenGlRenderEngineBackend> m_uiRenderingBackend;

		uint32_t m_width;
		uint32_t m_height;
	};
}
