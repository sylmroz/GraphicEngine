#pragma once

#include "../../../Common/EntityByVertexTypeManager.hpp"
#include "../../../Core/Math/ImageUtils.hpp"
#include "../../../Scene/Resources/Mesh.hpp"
#include "../../../Services/CameraControllerManager.hpp"
#include "../Shaders/Models/WireframeModelDescriptor.hpp"
#include <algorithm>

#undef max

namespace GraphicEngine::Engines::Graphic
{
	template <typename VertexType, template <typename> typename VertexBuffer>
	struct WireframeVertexBufferCollection
	{
		using vertex_type = VertexType;
		std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer;
		Shaders::WireframeModelDescriptor modelDescriptor;
		std::shared_ptr<Scene::Mesh<VertexType>> mesh;
	};

	template <template <typename> typename VertexBuffer, template <typename> typename UniformBuffer, template <typename> typename UniformBufferDynamic, typename... Args>
	class WireframeGraphicPipeline
	{
		template <typename VertexType>
		using VWireframeVertexBufferCollection = WireframeVertexBufferCollection<VertexType, VertexBuffer>;
	public:
		template <typename VertexType>
		void addVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto vertexBufferCollection = std::make_shared<WireframeVertexBufferCollection<VertexType, VertexBuffer>>();
			vertexBufferCollection->vertexBuffer = vertexBuffer;
			glm::vec3 color = mesh->getMaterial().solidColor;
			color = Core::changeContrast(color, glm::vec3(1.2f));
			vertexBufferCollection->modelDescriptor.wireframeColor = color;
			vertexBufferCollection->mesh = mesh;
			m_vertexBufferCollection->addEntity(vertexBufferCollection);
		}

		template <typename VertexType>
		void eraseVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh)
		{
			auto it = m_vertexBufferCollection->findIf<VertexType>([&](std::shared_ptr<WireframeVertexBufferCollection<VertexType, VertexBuffer>> vertexBufferCollection)
			{
				return vertexBufferCollection->mesh == mesh;
			});
			m_vertexBufferCollection->eraseEntity<VertexType>(it);
		}

		template <typename VertexType>
		void eraseVertexBuffer(std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto it = m_vertexBufferCollection->findIf<VertexType>([&](std::shared_ptr<WireframeVertexBufferCollection<VertexType, VertexBuffer>> vertexBufferCollection)
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

	protected:

	protected:
		std::shared_ptr<UniformBuffer<glm::mat4>> m_cameraUniformBuffer;
		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<UniformBufferDynamic<Engines::Graphic::Shaders::WireframeModelDescriptor>> m_wireframeModelDescriptorUniformBuffer;
		std::shared_ptr<Common::EntityByVertexTypeManager<VWireframeVertexBufferCollection>> m_vertexBufferCollection = std::make_shared<Common::EntityByVertexTypeManager<VWireframeVertexBufferCollection>>();
	};
}