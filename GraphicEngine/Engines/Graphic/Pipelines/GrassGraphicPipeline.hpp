#pragma once

#include "../Shaders/Models/CameraMatrices.hpp"
#include "../Shaders/Models/GrassParameters.hpp"
#include "../Shaders/Models/ModelMatrices.hpp"
#include "../Shaders/Models/GrassMaterial.hpp"
#include "GraphicPipeline.hpp"

namespace GraphicEngine::Engines::Graphic
{
	template <typename VertexType, template <typename> typename VertexBuffer>
	struct GrassVertexBufferCollection
	{
		using vertex_type = VertexType;
		std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer;
		Shaders::ModelMartices modelDescriptor;
		std::shared_ptr<Scene::Mesh<VertexType>> mesh;
	};

	template <template <typename> typename VertexBuffer, template <typename> typename UniformBuffer, template <typename> typename UniformBufferDynamic, typename... Args>
	class GrassGraphicPipeline : public GraphicPipeline<GrassGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, Args...>, GrassVertexBufferCollection, VertexBuffer, UniformBuffer, UniformBufferDynamic, Args...>
	{
		template <typename VertexType>
		using VGrassVertexBufferCollection = GrassVertexBufferCollection<VertexType, VertexBuffer>;
	public:
		template <typename VertexType>
		std::shared_ptr<VGrassVertexBufferCollection<VertexType>> produceVertexBufferCollection(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto vertexBufferCollection = std::make_shared<VGrassVertexBufferCollection<VertexType>>();
			vertexBufferCollection->vertexBuffer = vertexBuffer;
			vertexBufferCollection->mesh = mesh;
			return vertexBufferCollection;
		}

		virtual void draw(Args... args) = 0;

	protected:
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> m_cameraUniformBuffer;
		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<UniformBufferDynamic<Shaders::ModelMartices>> m_modelDescriptorUniformBuffer;
		std::shared_ptr<UniformBufferDynamic<Shaders::GrassParameters>> m_grassParametersUniformBuffer;
		std::shared_ptr<UniformBufferDynamic<Shaders::GrassMaterial>> m_materialUniformBuffer;
	};
}
