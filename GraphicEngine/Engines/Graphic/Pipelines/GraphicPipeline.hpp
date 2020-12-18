#pragma once

#include "../../../Common/EntityByVertexTypeManager.hpp"
#include "../../../Core/Math/ImageUtils.hpp"
#include "../../../Scene/Resources/Mesh.hpp"
#include "../../../Services/CameraControllerManager.hpp"
#include <algorithm>

namespace GraphicEngine::Engines::Graphic
{
	template <typename GraphicPipelineImpl, template <typename, template <typename> typename> typename VertexBufferCollection, template <typename> typename VertexBuffer, template <typename> typename UniformBuffer, template <typename> typename UniformBufferDynamic, typename... Args>
	class GraphicPipeline
	{
		template <typename VertexType>
		using VVertexBufferCollection = VertexBufferCollection<VertexType, VertexBuffer>;
	public:
		template <typename VertexType>
		void addVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto vertexBufferCollection = produceVertexBufferCollection(mesh, vertexBuffer);
			m_vertexBufferCollection->addEntity(vertexBufferCollection);
		}

		template <typename VertexType>
		void eraseVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh)
		{
			auto it = m_vertexBufferCollection->findIf<VertexType>([&](std::shared_ptr<VVertexBufferCollection<VertexType>> vertexBufferCollection)
			{
				return vertexBufferCollection->mesh == mesh;
			});
			m_vertexBufferCollection->eraseEntity<VertexType>(it);
		}

		template <typename VertexType>
		void eraseVertexBuffer(std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto it = m_vertexBufferCollection->findIf<VertexType>([&](std::shared_ptr<VVertexBufferCollection<VertexType>> vertexBufferCollection)
			{
				return vertexBufferCollection->vertexBuffer == vertexBuffer;
			});
			m_vertexBufferCollection->eraseEntity<VertexType>(it);
		}

		template <typename VertexType>
		void removeVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh)
		{

		}

		template <typename VertexType>
		void removeVertexBuffer(std::shared_ptr<VertexBuffer<VertexType>> mesh)
		{

		}

		virtual void draw(Args... args) = 0;

		template <typename VertexType>
		std::shared_ptr<VVertexBufferCollection<VertexType>> produceVertexBufferCollection(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			return static_cast<GraphicPipelineImpl*>(this)->produceVertexBufferCollection(mesh, vertexBuffer);
		}

	protected:
		std::shared_ptr<Common::EntityByVertexTypeManager<VVertexBufferCollection>> m_vertexBufferCollection = std::make_shared<Common::EntityByVertexTypeManager<VVertexBufferCollection>>();
	};
}