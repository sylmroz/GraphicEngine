#pragma once

#include "OpenGLGraphicPipeline.hpp"
#include "../../../Engines/Graphic/Shaders/Models/WireframeModelDescriptor.hpp"
#include "../../../Common/EntityByVertexTypeManager.hpp"
#include "../../../Services/CameraControllerManager.hpp"
#include "../../../Scene/Resources/Mesh.hpp"
#include "../../../Core/Math/ImageUtils.hpp"

namespace GraphicEngine::OpenGL
{
	template <typename VertexType>
	struct WireframeVertexBufferCollection
	{
		std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer;
		Engines::Graphic::Shaders::WireframeModelDescriptor modelDescriptor;
		std::shared_ptr<Scene::Mesh<VertexType>> mesh;
	};

	class OpenGLWireframeGraphicPipeline
	{
	public:
		OpenGLWireframeGraphicPipeline(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager);
		template <typename VertexType>
		void addVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			auto vertexBufferCollection = std::make_shared<WireframeVertexBufferCollection<VertexType>>();
			vertexBufferCollection->vertexBuffer = vertexBuffer;
			vertexBufferCollection->modelDescriptor.wireframeColor = Core::randomColor();
			vertexBufferCollection->mesh = mesh;
			m_vertexBufferCollection->addEntity(vertexBufferCollection);
		}

		void draw();
	private:
		std::unique_ptr<OpenGLShaderProgram> m_shaderProgram;
		std::shared_ptr<UniformBuffer<glm::mat4>> m_cameraUniformBuffer;
		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::WireframeModelDescriptor>> m_wireframeModelDescriptorUniformBuffer;
		std::shared_ptr<Common::EntityByVertexTypeManager<WireframeVertexBufferCollection>> m_vertexBufferCollection;
	};
}
