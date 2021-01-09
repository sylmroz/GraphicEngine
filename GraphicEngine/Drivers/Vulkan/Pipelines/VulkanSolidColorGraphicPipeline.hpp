#pragma once

#include "../../../Engines/Graphic/Pipelines/SolidColorGraphicPipeline.hpp"
#include "VulkanGraphicPipeline.hpp"
#include "../VulkanShaderStorageBufferObject.hpp"

#include <array>

namespace GraphicEngine::Vulkan
{
	class VulkanSolidColorGraphicPipeline : public Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>
	{
	public:
		VulkanSolidColorGraphicPipeline(std::shared_ptr<VulkanFramework> framework, std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::CameraMatrices>> cameraUniformBuffer,
			std::shared_ptr<UniformBuffer<Engines::Graphic::Shaders::Eye>> eyePositionUniformBuffer,
			std::shared_ptr<ShaderStorageBufferObject<Engines::Graphic::Shaders::DirectionalLight>> directionalLight, std::shared_ptr<ShaderStorageBufferObject<Engines::Graphic::Shaders::PointLight>> pointLights,
			std::shared_ptr<ShaderStorageBufferObject<Engines::Graphic::Shaders::SpotLight>> spotLight,
		std::shared_ptr<Services::CameraControllerManager> cameraControllerManager);

		template <typename VertexType>
		void addVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh, std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>::addVertexBuffer(mesh, vertexBuffer);
			addUniformBuffer();
		}

		template <typename VertexType>
		void eraseVertexBuffer(std::shared_ptr<Scene::Mesh<VertexType>> mesh)
		{
			Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>::eraseVertexBuffer(mesh);
			deleteUniformBuffer();
		}

		template <typename VertexType>
		void eraseVertexBuffer(std::shared_ptr<VertexBuffer<VertexType>> vertexBuffer)
		{
			Engines::Graphic::SolidColorGraphicPipeline<VertexBuffer, UniformBuffer, UniformBufferDynamic, vk::UniqueCommandBuffer&, int>::eraseVertexBuffer(vertexBuffer);
			deleteUniformBuffer();
		}

		virtual void draw(vk::UniqueCommandBuffer& commandBuffer, int index) override;

		void updateDynamicUniforms();

	private:
		void addUniformBuffer();
		void deleteUniformBuffer();

	private:
		std::shared_ptr<VulkanFramework> m_framework;
		std::shared_ptr<Common::EntityByVertexTypeManager<VulkanGraphicPipelineInfo>> m_vulkanGraphicPipelines;
		uint32_t alignedSize;
		std::vector<Engines::Graphic::Shaders::SolidColorModelDescriptor> m_solidColorModelDescriptors;
		std::shared_ptr<ShaderStorageBufferObject<Engines::Graphic::Shaders::DirectionalLight>> m_directionalLight;
		std::shared_ptr<ShaderStorageBufferObject<Engines::Graphic::Shaders::PointLight>> m_pointLights;
		std::shared_ptr<ShaderStorageBufferObject<Engines::Graphic::Shaders::SpotLight>> m_spotLight;

	private:
		vk::UniqueDescriptorPool m_descriptorPool;
		vk::UniqueDescriptorSetLayout m_descriptorSetLayout;
		std::vector<vk::UniqueDescriptorSet> m_descriptorSets;
	};
}