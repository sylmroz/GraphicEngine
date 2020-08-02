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

		template <template<typename> typename VertexBufferFactory, template<typename> typename VertexBuffer, typename... Args>
		std::vector<std::shared_ptr<VertexBuffer<Vertex>>> compile(Args&... args)
		{
			std::vector<std::shared_ptr<VertexBuffer<Vertex>>> vertexBuffers;
			vertexBuffers.reserve(m_meshes.size());
			for (auto& mesh : m_meshes)
			{
				vertexBuffers.push_back(mesh->compile<VertexBufferFactory<Vertex>, VertexBuffer<Vertex>>(args...));
			}

			return vertexBuffers;
		}

	protected:
		std::vector<std::shared_ptr<Mesh<Vertex>>> m_meshes;
		std::string m_name;
	};
}