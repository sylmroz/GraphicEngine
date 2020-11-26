#pragma once

#include <memory>
#include <vector>

namespace GraphicEngine::Common
{
	template <template<typename> typename BasicVertexBuffer, typename Vertex, typename... Args>
	class VertexBufferFactory
	{
	public:
		static std::shared_ptr<BasicVertexBuffer<Vertex>> produceVertexBuffer(Args... args, const std::vector<Vertex>& vertices)
		{
			return std::make_shared<BasicVertexBuffer<Vertex>>(args..., vertices);
		}

		static std::shared_ptr<BasicVertexBuffer<Vertex>> produceVertexBuffer(Args... args, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
		{
			return std::make_shared<BasicVertexBuffer<Vertex>>(args..., vertices, indices);
		}

		static std::shared_ptr<BasicVertexBuffer<Vertex>> produceVertexBuffer(Args... args, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& edges)
		{
			return std::make_shared<BasicVertexBuffer<Vertex>>(args..., vertices, indices, edges);
		}
	};
}