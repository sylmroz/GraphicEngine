#pragma once

#define NOMINMAX

#include "../Core/EventManager.hpp"
#include "../Core/Logger.hpp"
#include "Vertex.hpp"
#include "Camera.hpp"
//#include "../Scene/Resources/Mesh.hpp"
#include "../Scene/Resources/Model.hpp"
#include "../Modules/Assimp/AssimpModelImporter.hpp"

namespace GraphicEngine
{
	struct Light
	{
		glm::vec3 lightPosition;
		glm::vec3 eyePosition;
	};

	class RenderingEngine
	{
	public:
		RenderingEngine(std::shared_ptr<Common::Camera> camera,
			std::shared_ptr<Core::EventManager> eventManager) :
			m_camera(camera),
			m_eventManager(eventManager)
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

			m_model = std::make_shared<Scene::Model<Modules::AssimpModelImporter, Common::VertexPN>>("C:/teapot.obj");
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

		//std::shared_ptr<Scene::Mesh<Common::VertexPCTc>> m_mesh;

		std::shared_ptr<Scene::Model<Modules::AssimpModelImporter, Common::VertexPN>> m_model;
		Light light;
	};
}

