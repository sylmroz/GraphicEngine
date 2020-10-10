#pragma once

#define NOMINMAX

#include "../Core/EventManager.hpp"
#include "../Core/Logger.hpp"
#include "Vertex.hpp"
#include "Camera.hpp"
#include "../Scene/Resources/Model.hpp"
#include "../Modules/Assimp/AssimpModelImporter.hpp"
#include "../Core/Utils/ObjectConverter.hpp"

#include "../Engines/Graphic/3D/ObjectGenerators/PlaneGenerator.hpp"
#include "../Engines//Graphic/Shaders/Models/Light.hpp"

namespace GraphicEngine
{
	class RenderingEngine
	{
	public:
		RenderingEngine(std::shared_ptr<Common::Camera> camera,
			std::shared_ptr<Core::EventManager> eventManager,
			std::shared_ptr<Core::Configuration> cfg) :
			m_camera(camera),
			m_eventManager(eventManager),
			m_cfg(cfg)
		{
			// TO DO
			/*std::vector<Common::VertexPCTc> vertices =
			{
				{glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0,0,1), glm::vec2(1.0f, 1.0f)},
				{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1,0,0), glm::vec2(1.0f, 0.0f)},
				{glm::vec3(-0.5f, -0.5f, 0.0f),glm::vec3(0,1,0), glm::vec2(0.0f, 0.0f)},
				{glm::vec3(-0.5f,  0.5f, 0.0f),glm::vec3(1,1,0), glm::vec2(0.0f, 1.0f)}
			};

			std::vector<Scene::Face> faces =
			{
				Scene::Face({0,1,3}),
				Scene::Face({1,2,3})
			};

			m_mesh = std::make_shared<Scene::Mesh<Common::VertexPCTc>>(vertices, faces);*/

			auto plane = Engines::Graphic::PlaneGenerator<Common::VertexPN>{}.getModel(glm::vec2(-10.0f), glm::vec2(10.0f), glm::ivec2(2), Engines::Graphic::GeneratingPosition::Corner, Engines::Graphic::TriangleDirection::Clockwise);
			m_models = Modules::AssimpModelImporter<Common::VertexPN>{}.read(m_cfg->getProperty<std::string>("scene:object:path"));
			m_models.front()->setScale(m_cfg->getProperty<float>("scene:object:scale"));
			m_models.front()->setRotate(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:rotate")));
			m_models.front()->setPosition(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:position")));
			m_models.push_back(plane);

			light.lightPosition = glm::vec3(100.0, 100.0, 100.0);

			
		}

		virtual bool drawFrame() = 0;
		virtual void init(size_t width, size_t height) = 0;
		virtual void resizeFrameBuffer(size_t width, size_t height) = 0;
		virtual void cleanup() = 0;

		void setCamera(std::shared_ptr<Common::Camera> camera) { m_camera = camera; }

		virtual ~RenderingEngine() = default;
	protected:
		std::shared_ptr<Common::Camera> m_camera;
		std::shared_ptr<Core::EventManager> m_eventManager;
		std::shared_ptr<Core::Configuration> m_cfg;

		std::vector<std::shared_ptr<Scene::Model<Common::VertexPN>>> m_models;
		Engines::Graphic::Shaders::Light light;
	};
}

