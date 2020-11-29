#pragma once

#include "../Core/Utils/UniqueIdentifier.hpp"

namespace GraphicEngine::Common
{
	template <typename BasicVertexBuffer, typename... Args>
	class VertexBuffer : public Core::Utils::UniqueIdentifier
	{
	public:
		// using VertexType = BasicVertexBuffer::VertexType;

		void bind(Args... args)
		{
			static_cast<BasicVertexBuffer*>(this)->bind(args...);
		}

		void draw(Args... args)
		{
			static_cast<BasicVertexBuffer*>(this)->draw(args...);
		}

		void drawElements(Args... args)
		{
			static_cast<BasicVertexBuffer*>(this)->drawElements(args...);
		}

		void drawEdges(Args... args)
		{
			static_cast<BasicVertexBuffer*>(this)->drawEdges(args...);
		}

		void unbind(Args... args)
		{
			static_cast<BasicVertexBuffer*>(this)->unbind(args...);
		}
	};
}
