#pragma once

#include "../Shaders/Models/SolidColorModelDescriptor.hpp"
#include "../Shaders/Models/Light.hpp"
#include "../../../Common/EntityByVertexTypeManager.hpp"
#include "../../../Core/Math/ImageUtils.hpp"
#include "../../../Scene/Resources/Mesh.hpp"
#include "../../../Services/CameraControllerManager.hpp"

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
	class SolidColorGraphicPipeline
	{
		template <typename VertexType>
		using VSolidColorVertexBufferCollection = SolidColorVertexBufferCollection<VertexType, VertexBuffer>;
	public:
		SolidColorGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager) :
			m_cameraControllerManager{ cameraControllerManager }
		{
			m_vertexBufferCollection = std::make_shared<Common::EntityByVertexTypeManager<VSolidColorVertexBufferCollection>>();
		}

		template <typename VertexType>
		void addVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto vertexBufferCollection = std::make_shared<VSolidColorVertexBufferCollection<VertexType>>();
			vertexBufferCollection->vertexBuffer = vertexBuffer;
			vertexBufferCollection->modelDescriptor.color = mesh->getMaterial().solidColor;
			vertexBufferCollection->mesh = mesh;
			m_vertexBufferCollection->addEntity(vertexBufferCollection);
		}

		virtual void draw(Args... args) = 0;

	protected:
		std::shared_ptr<UniformBuffer<glm::mat4>> m_cameraUniformBuffer;
		std::shared_ptr<UniformBuffer<glm::vec3>> m_eyePositionUniformBuffer;
		std::shared_ptr<UniformBuffer<Shaders::Light>> m_ligthUniformBuffer;
		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<UniformBufferDynamic<Shaders::SolidColorModelDescriptor>> m_solidColorUniformBuffer;
		std::shared_ptr<Common::EntityByVertexTypeManager<VSolidColorVertexBufferCollection>> m_vertexBufferCollection;
	};
};
