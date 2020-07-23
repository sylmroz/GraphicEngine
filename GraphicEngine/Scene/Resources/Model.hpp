#pragma once

#include "Mesh.hpp"
#include "../../Common/ModelImporter.hpp"

namespace GraphicEngine::Scene
{
	template <template<typename> typename ModelImporterImpl, typename Vertex>
	class Model
	{
	public:
		Model() {}
		Model(const std::string& path)
		{
			read(path);
		}

		void read(const std::string& path)
		{
			std::tie(m_meshes, m_name) = std::move(Common::ModelImporter<ModelImporterImpl<Vertex>, Vertex>{}(path));
		}


	protected:
		std::vector<std::shared_ptr<Mesh<Vertex>>> m_meshes;
		std::string m_name;
	};
}