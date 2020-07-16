#pragma once

#include <string>

namespace GraphicEngine::Common
{
	template <typename ModelReaderImpl, typename Vertex, typename VertexArray>
	class ModelReader
	{
	public:
		ModelReader() {}
		ModelReader(const std::string& path) {}

		auto& operator()()
		{
			auto& [vertices, indexes] = ModelReaderImpl::read(path);
			return std::make_tuple(std::move(vertices), std::move(indexes), std::move(name));
		}
		
	};
}