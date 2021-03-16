#pragma once

#include "../Common/EntityByVertexTypeManager.hpp"
#include "../Core/Configuration.hpp"
#include "../Core/Logger.hpp"
#include "../Scene/Resources/Model.hpp"

namespace GraphicEngine::Services
{
	using ModelEntityContainer = Common::EntityByVertexTypeManager<Scene::Model>;
	class ModelManager
	{
	public:
		ModelManager(std::shared_ptr<Core::Configuration> cfg, std::unique_ptr<Core::Logger<ModelManager>> logger);

		template <typename VertexType>
		void addModel(std::shared_ptr<Scene::Model<VertexType>> model)
		{
			m_modelContainer->addEntity(model);
		}

		template <typename VertexType>
		void eraseeModel(std::shared_ptr<Scene::Model<VertexType>> model)
		{
			auto it = m_modelContainer->findIf<VertexType>([&](std::shared_ptr<Scene::Model<VertexType>> modelIn)
			{
				return *model == *modelIn;
			});
			m_modelContainer->eraseEntity(it);
		}

		std::shared_ptr<ModelEntityContainer> getModelEntityContainer();
	protected:
	private:
		std::shared_ptr<ModelEntityContainer> m_modelContainer = std::make_shared<ModelEntityContainer>();;
		std::unique_ptr<Core::Logger<ModelManager>> m_logger;
	};
}