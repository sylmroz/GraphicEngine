#pragma once

#include "GraphicPipeline.hpp"
#include "../Shaders/Models/ModelMatrices.hpp"
#include "../Shaders/Models/CameraMatrices.hpp"

namespace GraphicEngine::Engines::Graphic
{
	template <typename VertexType, template <typename> typename VertexBuffer>
	struct NormalDebugVertexBufferCollection
	{
		using vertex_type = VertexType;
		std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer;
		Shaders::ModelMartices modelDescriptor;
		std::shared_ptr<Scene::Mesh<VertexType>> mesh;
	};

	template <template <typename> typename VertexBuffer, template <typename> typename UniformBuffer, template <typename> typename UniformBufferDynamic, typename... Args>
	class NormalDebugGraphicPipeline : public GraphicPipeline<NormalDebugGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, Args...>, NormalDebugVertexBufferCollection, VertexBuffer, UniformBuffer, UniformBufferDynamic, Args...>
	{
		template <typename VertexType>
		using VNormalDebugVertexBufferCollection = NormalDebugVertexBufferCollection<VertexType, VertexBuffer>;
	public:
		template <typename VertexType>
		std::shared_ptr<VNormalDebugVertexBufferCollection<VertexType>> produceVertexBufferCollection(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto vertexBufferCollection = std::make_shared<VNormalDebugVertexBufferCollection<VertexType>>();
			vertexBufferCollection->vertexBuffer = vertexBuffer;
			vertexBufferCollection->mesh = mesh;
			return vertexBufferCollection;
		}

		virtual void draw(Args... args) = 0;

	protected:
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> m_cameraUniformBuffer;
		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<UniformBufferDynamic<Shaders::ModelMartices>> m_modelDescriptorUniformBuffer;
	};
}
