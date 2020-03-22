#ifndef GRAPHIC_ENGINE_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_RENDERING_ENGINE_HPP

#include "Window.hpp"

#include "Vertex.hpp"

namespace GraphicEngine
{
	class RenderingEngine
	{
	public:
		RenderingEngine(std::shared_ptr<Window> window,
			std::shared_ptr<Common::Camera> camera) :
			_window(window),
			_camera(camera)
		{}

		virtual bool drawFrame() = 0;
		virtual void init(size_t width, size_t height) = 0;
		virtual void resizeFrameBuffer(size_t width, size_t height) = 0;
		virtual void cleanup() = 0;

		void setCamera(std::shared_ptr<Common::Camera> camera) { _camera = camera; }
	protected:
		std::shared_ptr<Window> _window;

		std::shared_ptr<Common::Camera> _camera;
		
		std::vector<Common::VertexPC> vertices =
		{
			{glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0,0,1)},
			{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1,0,0)},
			{glm::vec3(-0.5f, -0.5f, 0.0f),glm::vec3(0,1,0)},
			{glm::vec3(-0.5f,  0.5f, 0.0f),glm::vec3(1,1,0)}
		};

		std::vector<uint32_t> indices =
		{
			0,1,3,
			1,2,3
		};
		
	};
}

#endif // !GRAPHIC_ENGINE_RENDERING_ENGINE_HPP
