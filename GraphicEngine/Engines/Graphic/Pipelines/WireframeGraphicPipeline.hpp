#pragma once

#include "GraphicPipeline.hpp"
#include "../Shaders/Models/WireframeModelDescriptor.hpp"
#include "../Shaders/Models/CameraMatrices.hpp"

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
			vertexBufferCollection->mesh = mesh;
			vertexBufferCollection->modelDescriptor.wireframeColor = glm::vec4(Core::changeContrast(glm::vec3(vertexBufferCollection->mesh->getMaterial().solidColor), glm::vec3(1.2f)), 1.0f);
			return vertexBufferCollection;
		}

		virtual void draw(Args... args) = 0;

	protected:
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> m_cameraUniformBuffer;
		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<UniformBufferDynamic<Engines::Graphic::Shaders::WireframeModelDescriptor>> m_wireframeModelDescriptorUniformBuffer;
	};
}