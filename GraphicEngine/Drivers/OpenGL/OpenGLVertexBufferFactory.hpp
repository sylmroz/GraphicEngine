#pragma once

#include "../../Common/VertexBufferFactory.hpp"
#include "OpenGLVertexBuffer.hpp"

namespace GraphicEngine::OpenGL
{
	template <typename Vertex>
	using VertexBufferFactory = Common::VertexBufferFactory<VertexBuffer, Vertex>;
}