#ifndef GRAPHIC_ENGINE_RENDERING_ENGINE_HPP
#define GRAPHIC_ENGINE_RENDERING_ENGINE_HPP

#include "Window.hpp"

#include "Vertex.hpp"

namespace GraphicEngine
{
	class RenderingEngine
	{
	public:
		RenderingEngine(std::shared_ptr<Window> window) :
			_window(window) {}

		virtual bool drawFrame() = 0;
		virtual void init(size_t width, size_t height) = 0;
		virtual void resizeFrameBuffer(size_t width, size_t height) = 0;
		virtual void cleanup() = 0;
	protected:
		std::shared_ptr<Window> _window;

		std::vector<Common::VertexP> vertices =
		{
			Common::VertexP(glm::vec3(0.5f,  0.5f, 0.0f)),
			Common::VertexP(glm::vec3(0.5f, -0.5f, 0.0f)),
			Common::VertexP(glm::vec3(-0.5f, -0.5f, 0.0f)),
			Common::VertexP(glm::vec3(-0.5f,  0.5f, 0.0f))
		};

		std::vector<uint32_t> indices =
		{
			0,1,3,
			1,2,3
		};
		
	};
}

#endif // !GRAPHIC_ENGINE_RENDERING_ENGINE_HPP
