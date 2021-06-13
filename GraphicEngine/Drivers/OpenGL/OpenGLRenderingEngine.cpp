#include "OpenGLRenderingEngine.hpp"

#include "../../Core/IO/FileReader.hpp"
#include "../../Core/IO/FileSystem.hpp"

#include "../../Common/ShaderEnums.hpp"

#include "OpenGLTextureFactory.hpp"
#include "OpenGLVertexBufferFactory.hpp"

GraphicEngine::OpenGL::OpenGLRenderingEngine::OpenGLRenderingEngine(
	std::shared_ptr<Services::ServicesManager> servicesManager,
	std::shared_ptr<Core::EventManager> eventManager,
	std::shared_ptr<Core::Timer> timer,
	std::shared_ptr<Common::UI> ui,
	std::shared_ptr<Core::Configuration> cfg,
	std::unique_ptr<Core::Logger<OpenGLRenderingEngine>> logger) :
	m_logger(std::move(logger)),
	RenderingEngine(servicesManager, eventManager, timer, ui, cfg)
{
	m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Create OpenGL rendering engine instance.");
}

bool GraphicEngine::OpenGL::OpenGLRenderingEngine::drawFrame()
{
	// Create shadow maps
	if (m_renderingOptionsManager->renderingOptions.shadowRendering.directional)
	{
		glClearColor(m_viewportManager->backgroudColor.r, m_viewportManager->backgroudColor.g, m_viewportManager->backgroudColor.b, m_viewportManager->backgroudColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_shadowMapGraphicPipeline->draw();
	}

	if (m_renderingOptionsManager->renderingOptions.shadowRendering.spot)
	{
		glClearColor(m_viewportManager->backgroudColor.r, m_viewportManager->backgroudColor.g, m_viewportManager->backgroudColor.b, m_viewportManager->backgroudColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_spotLightshadowMapGraphicPipeline->draw();
	}

	if (m_renderingOptionsManager->renderingOptions.shadowRendering.point)
	{
		glClearColor(m_viewportManager->backgroudColor.r, m_viewportManager->backgroudColor.g, m_viewportManager->backgroudColor.b, m_viewportManager->backgroudColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pointLightshadowMapGraphicPipeline->draw();
	}

	// Render Normal Scene
	glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_ui->nextFrame();

	auto viewMatrix = m_cameraControllerManager->getActiveCamera()->getViewMatrix();
	auto projectionMatrix = m_cameraControllerManager->getActiveCamera()->getProjectionMatrix();
	Engines::Graphic::Shaders::CameraMatrices cameraMatrices{ viewMatrix, projectionMatrix };
	m_cameraUniformBuffer->update(&cameraMatrices);

	auto eyePosition = m_cameraControllerManager->getActiveCamera()->getPosition();
	Engines::Graphic::Shaders::Eye eye{ glm::vec4(eyePosition, 1.0) };
	m_eyeUniformBuffer->update(&eye);

	m_grassGraphicPipeline->draw();
	
	if (m_viewportManager->displayNormal)
		m_normalDebugGraphicPipeline->draw();
	if (m_viewportManager->displayWireframe)
		m_wireframeGraphicPipeline->draw();
	if (m_viewportManager->displaySolid)
		m_solidColorGraphicPipeline->draw();
	if (m_viewportManager->displaySkybox)
		m_skyboxGraphicPipeline->draw();

	m_ui->drawUi();
	m_uiRenderingBackend->renderData();

	return false;
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::init(size_t width, size_t height)
{
	m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Initialize OpenGL rendering engine instance.");
	
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("OpenGL initialization failed!");
	}
	resizeFrameBuffer(width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	try
	{
		m_directionalLightDepthTexture = std::make_shared<TextureDepthArray>(2048, 2048, 5);
		m_spotLightdepthTexture = std::make_shared<TextureDepthArray>(256, 256, 5);
		m_pointightdepthTexture = std::make_shared<TextureCubeDepthArray>(256, 256, 5);

		m_wireframeGraphicPipeline = std::make_unique<OpenGLWireframeGraphicPipeline>(m_cameraControllerManager);
		m_solidColorGraphicPipeline = std::make_unique<OpenGLSolidColorGraphicPipeline>(m_cameraControllerManager, m_directionalLightDepthTexture, m_spotLightdepthTexture, m_pointightdepthTexture);
		m_normalDebugGraphicPipeline = std::make_unique<OpenGLNormalDebugGraphicPipeline>(m_cameraControllerManager);
		m_grassGraphicPipeline = std::make_unique<OpenGLGrassGraphicPipeline>(m_cameraControllerManager, m_directionalLightDepthTexture, m_spotLightdepthTexture, m_pointightdepthTexture);
		m_skyboxGraphicPipeline = std::make_unique<OpenGLSkyboxGraphicPipeline>(m_cfg->getProperty<std::string>("scene:skybox:texture path"));

		Engines::Graphic::Shaders::LightSpaceMatrixArray lightSpaceMatrixArray;
		for (auto& dirLight : m_lightManager->getDirectionalLights())
		{
			lightSpaceMatrixArray.data.push_back(dirLight.lightSpace);
		}
		m_shadowMapGraphicPipeline = std::make_unique<OpenGLShadowMapGraphicPipeline>(m_directionalLightDepthTexture, lightSpaceMatrixArray);

		Engines::Graphic::Shaders::LightSpaceMatrixArray spotLightSpaceMatrixArray;
		Engines::Graphic::Shaders::LightPositionFarPlaneArray spotLightPositionFarPlaneArray;
		for (auto& spotLight : m_lightManager->getSpotLights())
		{
			spotLightSpaceMatrixArray.data.push_back(spotLight.lightSpace);
			spotLightPositionFarPlaneArray.data.push_back(glm::vec4(glm::vec3(spotLight.position), 50.0f));
		}
		m_spotLightshadowMapGraphicPipeline = std::make_unique<OpenGLShadowMapGraphicPipeline>(m_spotLightdepthTexture, spotLightSpaceMatrixArray, LightTypeShadow::spot, spotLightPositionFarPlaneArray);

		Engines::Graphic::Shaders::LightSpaceMatrixArray pointLightSpaceMatrixArray;
		Engines::Graphic::Shaders::LightPositionFarPlaneArray pointLightPositionFarPlaneArray;
		for (auto& pointLight : m_lightManager->getPointLights())
		{
			for (auto& lightSpaceMatrix : pointLight.getLightSpaceMatrices())
			{
				pointLightSpaceMatrixArray.data.push_back(lightSpaceMatrix);
			}
			pointLightPositionFarPlaneArray.data.push_back(glm::vec4(glm::vec3(pointLight.position), 25.0f));
		}
		m_pointLightshadowMapGraphicPipeline = std::make_unique<OpenGLShadowMapGraphicPipeline>(m_pointightdepthTexture, pointLightSpaceMatrixArray, LightTypeShadow::point, pointLightPositionFarPlaneArray);

		m_cameraUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>>(ShaderBinding::Global_CameraMatrices);
		m_eyeUniformBuffer = std::make_unique<UniformBuffer<Engines::Graphic::Shaders::Eye>>(ShaderBinding::Global_Eye);
		m_timeUniformBuffer = std::make_unique<UniformBuffer<Engines::Graphic::Shaders::Time>>(ShaderBinding::Global_Time);
		m_timer->onCurrentTimeUpdate([&](float timestamp)
			{
				// Convert to seconds
				Engines::Graphic::Shaders::Time t(timestamp/10000000);
				m_timeUniformBuffer->update(&t);
			});
		m_windParametersUniformBuffer = std::make_unique<UniformBuffer<Engines::Graphic::Shaders::WindParameters>>(ShaderBinding::Global_WindParameters);
		auto windParameters = m_windManager->getWindParameters();
		m_windParametersUniformBuffer->update(&windParameters);
		m_windManager->onUpdateWindParameters([&](Engines::Graphic::Shaders::WindParameters windParameters)
			{
				m_windParametersUniformBuffer->update(&windParameters);
			});

		m_renderingOptionsUniformBuffer = std::make_shared<UniformBuffer<Engines::Graphic::Shaders::RenderingOptions>>(ShaderBinding::Global_RenderingOptions);
		m_renderingOptionsUniformBuffer->update(&m_renderingOptionsManager->renderingOptions);
		m_renderingOptionsManager->onUpdateOptions([&](Engines::Graphic::Shaders::RenderingOptions renderingOptions) 
			{
				m_renderingOptionsUniformBuffer->update(&m_renderingOptionsManager->renderingOptions);
			});

		m_directionalLight = std::make_shared<ShaderStorageBufferObject<Engines::Graphic::Shaders::DirectionalLight>>(ShaderBinding::Global_DirectionalLight);
		m_pointLights = std::make_shared<ShaderStorageBufferObject<Engines::Graphic::Shaders::PointLight>>(ShaderBinding::Global_PointLight);
		m_spotLight = std::make_shared<ShaderStorageBufferObject<Engines::Graphic::Shaders::SpotLight>>(ShaderBinding::Global_SpotLight);

		m_directionalLight->update(m_lightManager->getDirectionalLights());
		m_lightManager->onUpdateDirectiionalLight([&](uint32_t index, Engines::Graphic::Shaders::DirectionalLight light)
		{
			m_directionalLight->update(light, index);
			m_shadowMapGraphicPipeline->updateLight(light.lightSpace, index);
		});
		m_lightManager->onUpdateDirectiionalLights([&](std::vector<Engines::Graphic::Shaders::DirectionalLight> lights)
		{
			m_directionalLight->update(lights);
			Engines::Graphic::Shaders::LightSpaceMatrixArray lightSpaceMatrixArray;
			for (auto& dirLight : lights)
			{
				lightSpaceMatrixArray.data.push_back(dirLight.lightSpace);
			}
			m_shadowMapGraphicPipeline->updateLights(lightSpaceMatrixArray);
		});

		m_pointLights->update(m_lightManager->getPointLights());
		m_lightManager->onUpdatePointLight([&](uint32_t index, Engines::Graphic::Shaders::PointLight light)
		{
			m_pointLights->update(light, index);
			m_pointLightshadowMapGraphicPipeline->updateLight(light.getLightSpaceMatrices(), index, glm::vec4(glm::vec3(light.position), 25.0f));
		});
		m_lightManager->onUpdatePointLights([&](std::vector<Engines::Graphic::Shaders::PointLight> lights)
		{
			m_pointLights->update(lights);
			Engines::Graphic::Shaders::LightSpaceMatrixArray pointLightSpaceMatrixArray;
			Engines::Graphic::Shaders::LightPositionFarPlaneArray pointLightPositionFarPlaneArray;
			for (auto& pointLight : lights)
			{
				for (auto& lightSpaceMatrix : pointLight.getLightSpaceMatrices())
				{
					pointLightSpaceMatrixArray.data.push_back(lightSpaceMatrix);
				}
				pointLightPositionFarPlaneArray.data.push_back(glm::vec4(glm::vec3(pointLight.position), 25.0f));
			}
			m_pointLightshadowMapGraphicPipeline->updateLights(pointLightSpaceMatrixArray, pointLightPositionFarPlaneArray);
		});

		m_spotLight->update(m_lightManager->getSpotLights());
		m_lightManager->onUpdateSpotlLight([&](uint32_t index, Engines::Graphic::Shaders::SpotLight light)
		{
			m_spotLight->update(light, index);
			m_spotLightshadowMapGraphicPipeline->updateLight(light.lightSpace, index, glm::vec4(glm::vec3(light.position), 50.0f));
		});
		m_lightManager->onUpdateSpotlLights([&](std::vector<Engines::Graphic::Shaders::SpotLight> lights)
		{
			m_spotLight->update(lights);
			Engines::Graphic::Shaders::LightSpaceMatrixArray spotLightSpaceMatrixArray;
			Engines::Graphic::Shaders::LightPositionFarPlaneArray spotLightPositionFarPlaneArray;
			for (auto& spotLight : lights)
			{
				spotLightSpaceMatrixArray.data.push_back(spotLight.lightSpace);
				spotLightPositionFarPlaneArray.data.push_back(glm::vec4(glm::vec3(spotLight.position), 50.0f));
			}
			m_spotLightshadowMapGraphicPipeline->updateLights(spotLightSpaceMatrixArray, spotLightPositionFarPlaneArray);
		});

		m_modelManager->getModelEntityContainer()->forEachEntity([&](auto model)
		{
			for (auto mesh : model->getMeshes())
			{
				auto vb = mesh->compile<VertexBufferFactory, VertexBuffer>();
				m_wireframeGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_solidColorGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_normalDebugGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_shadowMapGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_spotLightshadowMapGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_pointLightshadowMapGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
				m_grassGraphicPipeline->addVertexBuffer<decltype(mesh)::element_type::vertex_type>(mesh, vb);
			}
		});

		m_uiRenderingBackend = std::make_shared<GUI::ImGuiImpl::OpenGlRenderEngineBackend>();
		m_ui->addBackend(m_uiRenderingBackend);
	}

	catch (std::runtime_error& err)
	{
		m_logger->error(__FILE__, __LINE__, __FUNCTION__, err.what());
		throw (err);
	}
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::resizeFrameBuffer(size_t width, size_t height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, m_width, m_height);
}

void GraphicEngine::OpenGL::OpenGLRenderingEngine::cleanup()
{
}
