#pragma once

#include <string>
#include "../Scene/Resources/Mesh.hpp"

namespace GraphicEngine::Common
{
	template <typename ModelReaderImpl, typename Vertex>
	class ModelImporter
	{
	public:
		ModelImporter() {}
		ModelImporter(const std::string& path) :m_path{path} {}

		std::tuple<std::vector<std::shared_ptr<Scene::Mesh<Vertex>>>, std::string>& operator()()
		{
			return read(m_path);
		}

		std::tuple<std::vector<std::shared_ptr<Scene::Mesh<Vertex>>>, std::string>& operator()(const std::string& path)
		{
			return read(path);
		}

		std::tuple<std::vector<std::shared_ptr<Scene::Mesh<Vertex>>>, std::string>& read(const std::string& path)
		{
			return static_cast<ModelReaderImpl*>(this)->read(path);
		}

	protected:
		std::string m_path;
	};
}