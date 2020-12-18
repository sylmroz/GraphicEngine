#pragma once

#include "GraphicPipeline.hpp"
#include "../Shaders/Models/WireframeModelDescriptor.hpp"

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
	class WireframeGraphicPipeline : public GraphicPipeline<WireframeGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, Args...>, WireframeVertexBufferCollection, VertexBuffer, UniformBuffer, UniformBufferDynamic, Args...>
	{
		template <typename VertexType>
		using VWireframeVertexBufferCollection = WireframeVertexBufferCollection<VertexType, VertexBuffer>;
	public:
		template <typename VertexType>
		std::shared_ptr<VWireframeVertexBufferCollection<VertexType>> produceVertexBufferCollection(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto vertexBufferCollection = std::make_shared<WireframeVertexBufferCollection<VertexType, VertexBuffer>>();
			vertexBufferCollection->vertexBuffer = vertexBuffer;
			glm::vec3 color = mesh->getMaterial().solidColor;
			color = Core::changeContrast(color, glm::vec3(1.2f));
			vertexBufferCollection->modelDescriptor.wireframeColor = color;
			vertexBufferCollection->mesh = mesh;
			return vertexBufferCollection;
		}

		virtual void draw(Args... args) = 0;

	protected:
		std::shared_ptr<UniformBuffer<glm::mat4>> m_cameraUniformBuffer;
		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<UniformBufferDynamic<Engines::Graphic::Shaders::WireframeModelDescriptor>> m_wireframeModelDescriptorUniformBuffer;
	};
}