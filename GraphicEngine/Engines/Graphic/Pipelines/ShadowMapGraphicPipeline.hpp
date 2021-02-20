#pragma once

#include "GraphicPipeline.hpp"
#include "../Shaders/Models/ModelMatrix.hpp"
#include "../Shaders/Models/LightSpaceMatrix.hpp"
#include "../Shaders/Models/LightSpaceModelMatrices.hpp"
#include "../../../Services/LightManager.hpp"

namespace GraphicEngine::Engines::Graphic
{
	template <typename VertexType, template <typename> typename VertexBuffer>
	struct ShadowMapVertexBufferCollection
	{
		using vertex_type = VertexType;
		std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer;
		Shaders::LightSpaceModelMatrices modelDescriptor;
		std::shared_ptr<Scene::Mesh<VertexType>> mesh;
	};

	template <template <typename> typename VertexBuffer, template <typename> typename UniformBuffer, template <typename> typename UniformBufferDynamic, typename... Args>
	class ShadowMapGraphicPipeline : public GraphicPipeline<ShadowMapGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, Args...>, ShadowMapVertexBufferCollection, VertexBuffer, UniformBuffer, UniformBufferDynamic, Args...>
	{
		template <typename VertexType>
		using VShadowMapVertexBufferCollection = ShadowMapVertexBufferCollection<VertexType, VertexBuffer>;
	public:
		template <typename VertexType>
		std::shared_ptr<VShadowMapVertexBufferCollection<VertexType>> produceVertexBufferCollection(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto vertexBufferCollection = std::make_shared<VShadowMapVertexBufferCollection<VertexType>>();
			vertexBufferCollection->vertexBuffer = vertexBuffer;
			vertexBufferCollection->mesh = mesh;
			return vertexBufferCollection;
		}

		virtual void draw(Args... args) = 0;

	protected:
		std::shared_ptr<UniformBuffer<Shaders::ModelMatrix>> m_modelMatrix;
		std::shared_ptr<UniformBufferDynamic<Shaders::LightSpaceModelMatrices>> m_modelDescriptorUniformBuffer;
	};
}
