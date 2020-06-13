#pragma once

namespace GraphicEngine::Common
{
	template <typename BasicVertexBuffer, typename Vertex, typename... Args>
	class VertexBuffer
	{
	public:
		using VertexType = Vertex;
	public:
		void bind(Args... args)
		{
			static_cast<BasicVertexBuffer*>(this)->bind(args...);
		}

		void draw(Args... args)
		{
			static_cast<BasicVertexBuffer*>(this)->draw(args...);
		}

		void unbind(Args... args)
		{
			static_cast<BasicVertexBuffer*>(this)->unbind(args...);
		}
	};
}
