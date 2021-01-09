#pragma once

#include "GraphicPipeline.hpp"
#include "../Shaders/Models/SolidColorModelDescriptor.hpp"
#include "../Shaders/Models/Light.hpp"
#include "../Shaders/Models/Eye.hpp"
#include "../../../Core/Math/ImageUtils.hpp"
#include "../../../Services/CameraControllerManager.hpp"
#include "../Shaders/Models/CameraMatrices.hpp"

namespace GraphicEngine::Engines::Graphic
{
	template <typename VertexType, template <typename> typename VertexBuffer>
	struct SolidColorVertexBufferCollection
	{
		using vertex_type = VertexType;
		std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer;
		Shaders::SolidColorModelDescriptor modelDescriptor;
		std::shared_ptr<Scene::Mesh<VertexType>> mesh;
	};

	template <template <typename> typename VertexBuffer, template <typename> typename UniformBuffer, template <typename> typename UniformBufferDynamic, typename... Args>
	class SolidColorGraphicPipeline : public GraphicPipeline<SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, Args...>, SolidColorVertexBufferCollection, VertexBuffer, UniformBuffer, UniformBufferDynamic, Args...>
	{
		template <typename VertexType>
		using VSolidColorVertexBufferCollection = SolidColorVertexBufferCollection<VertexType, VertexBuffer>;
	public:
		SolidColorGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager) :
			m_cameraControllerManager{ cameraControllerManager } {}

		template <typename VertexType>
		std::shared_ptr<VSolidColorVertexBufferCollection<VertexType>> produceVertexBufferCollection(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto vertexBufferCollection = std::make_shared<VSolidColorVertexBufferCollection<VertexType>>();
			vertexBufferCollection->vertexBuffer = vertexBuffer;
			vertexBufferCollection->modelDescriptor.color = mesh->getMaterial().solidColor;
			vertexBufferCollection->mesh = mesh;
			return vertexBufferCollection;
		}

		virtual void draw(Args... args) = 0;

	protected:
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> m_cameraUniformBuffer;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::Eye>> m_eyePositionUniformBuffer;
		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<UniformBufferDynamic<Shaders::SolidColorModelDescriptor>> m_solidColorUniformBuffer;
	};
};
