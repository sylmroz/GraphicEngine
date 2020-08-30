#pragma once

#include <string>
#include "../Scene/Resources/Model.hpp"

namespace GraphicEngine::Common
{
	template <typename ModelReaderImpl, typename Vertex>
	class ModelImporter
	{
	public:
		ModelImporter() {}
		ModelImporter(const std::string& path) :m_path{path} {}

		std::vector<std::shared_ptr<Scene::Model<Vertex>>> operator()()
		{
			return read(m_path);
		}

		std::vector<std::shared_ptr<Scene::Model<Vertex>>> operator()(const std::string& path)
		{
			return read(path);
		}

		std::vector<std::shared_ptr<Scene::Model<Vertex>>> read(const std::string& path)
		{
			return static_cast<ModelReaderImpl*>(this)->read(path);
		}

	protected:
		std::string m_path;
	};
}