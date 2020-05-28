#pragma once

#include "../Core/EventManager.hpp"
#include "../Core/Logger.hpp"
#include "Vertex.hpp"
#include "Camera.hpp"

namespace GraphicEngine
{
	class RenderingEngine
	{
	public:
		RenderingEngine(std::shared_ptr<Common::Camera> camera,
			std::shared_ptr<Core::EventManager> eventManager) :
			m_camera(camera),
			m_eventManager(eventManager)
		{}

		virtual bool drawFrame() = 0;
		virtual void init(size_t width, size_t height) = 0;
		virtual void resizeFrameBuffer(size_t width, size_t height) = 0;
		virtual void cleanup() = 0;

		void setCamera(std::shared_ptr<Common::Camera> camera) { m_camera = camera; }

		virtual ~RenderingEngine() = default;
	protected:
		std::shared_ptr<Common::Camera> m_camera;

		std::shared_ptr<Core::EventManager> m_eventManager;

		std::vector<Common::VertexPCTc> vertices =
		{
			{glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0,0,1), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1,0,0), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(-0.5f, -0.5f, 0.0f),glm::vec3(0,1,0), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(-0.5f,  0.5f, 0.0f),glm::vec3(1,1,0), glm::vec2(0.0f, 1.0f)}
		};

		std::vector<uint32_t> indices =
		{
			0,1,3,
			1,2,3
		};

	};
}

