#include "OpenGLGrassGraphicPipeline.hpp"
#include "../../../Core/IO/FileReader.hpp"
#include "../../../Core/IO/FileSystem.hpp"

#include "../../../Common/ShaderEnums.hpp"

// temporary
#include "../../../Engines/Graphic/2D/WindGenerator.hpp"

GraphicEngine::OpenGL::OpenGLGrassGraphicPipeline::OpenGLGrassGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
	std::shared_ptr<Texture> directionalLighttShadowMap,
	std::shared_ptr<Texture> spotLightShadowMaps,
	std::shared_ptr<Texture> pointLightShadowMaps)
{
	using namespace Engines::Graphic::Shaders;

	m_cameraControllerManager = cameraControllerManager;
	m_directionalLighttShadowMap = directionalLighttShadowMap;
	m_spotLightShadowMaps = spotLightShadowMaps;
	m_pointLightShadowMaps = pointLightShadowMaps;

	OpenGLVertexShader vert(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("grass.vert").string()));
	OpenGLFragmentShader frag(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("grass.frag").string()));
	OpenGLGeometryShader geom(GraphicEngine::Core::IO::readFile<std::string>(Core::FileSystem::getOpenGlShaderPath("grass.geom").string()));

	m_shaderProgram = std::make_shared<OpenGLShaderProgram>(std::vector<OpenGLShader>{ vert, geom, frag });

	m_modelDescriptorUniformBuffer = std::make_shared<UniformBuffer<ModelMartices>>(ShaderBinding::Grass_ModelMartices, m_shaderProgram);
	m_materialUniformBuffer = std::make_shared<UniformBuffer<GrassMaterial>>(ShaderBinding::Grass_Material, m_shaderProgram);
	m_grassParametersUniformBuffer = std::make_shared<UniformBuffer<GrassParameters>>(ShaderBinding::Grass_GrassParameters, m_shaderProgram);

	m_shaderProgram->use();
	GrassMaterial grass
	{
		GrassColor{
			glm::vec4(0.063137f, 0.572f, 0.071372f, 1.0f),
			glm::vec4(0.07058f, 0.509803f, 0.078431f, 0.95f),
			glm::vec4(0.63137f, 0.89019f, 0.71372f, 0.95f),
			glm::vec4(0.07058f, 0.8901903f, 0.078431f, 0.95f)
		},
		GrassColor{
			glm::vec4(0.163137f, 0.572f, 0.071372f, 1.0f),
			glm::vec4(0.07058f, 0.509803f, 0.078431f, 0.95f),
			glm::vec4(0.63137f, 0.89019f, 0.71372f, 0.95f),
			glm::vec4(0.17058f, 0.95003f, 0.078431f, 0.95f)
		},
		2.0f
	};

	GrassParameters grassParameters(0.005f, 0.2f, 0.5f);
	m_grassParametersUniformBuffer->update(&grassParameters);

	m_materialUniformBuffer->update(&grass);

	auto windMap = Engines::Graphic::WindGenerator::generate(512, 0.5);
	m_windMap = std::make_unique<Texture2D>(windMap.data, 512, 512, 4);

	cv::imshow("", windMap);

	glUniform1i(glGetUniformLocation(m_shaderProgram->getShaderProgramId(), "windMap"), 0);
	m_windMap->use(0);

	glUniform1i(glGetUniformLocation(m_shaderProgram->getShaderProgramId(), "directionalLightShadowMap"), 1);
	m_directionalLighttShadowMap->use(1);

	glUniform1i(glGetUniformLocation(m_shaderProgram->getShaderProgramId(), "spotLightShadowMap"), 2);
	m_spotLightShadowMaps->use(2);

	glUniform1i(glGetUniformLocation(m_shaderProgram->getShaderProgramId(), "pointLightShadowMap"), 3);
	m_pointLightShadowMaps->use(3);
}

void GraphicEngine::OpenGL::OpenGLGrassGraphicPipeline::draw()
{
	m_shaderProgram->use();

	auto viewMatrix = m_cameraControllerManager->getActiveCamera()->getViewMatrix();

	m_vertexBufferCollection->forEachEntity([&](auto vertexBufferCollection)
		{
			vertexBufferCollection->modelDescriptor.modelMatrix = vertexBufferCollection->mesh->getModelMatrix();
			vertexBufferCollection->modelDescriptor.normalMatrix = glm::transpose(glm::inverse(vertexBufferCollection->modelDescriptor.modelMatrix));
			m_modelDescriptorUniformBuffer->update(&vertexBufferCollection->modelDescriptor);
			vertexBufferCollection->vertexBuffer->drawElements(GL_TRIANGLES);
		});
}

