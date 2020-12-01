#pragma once

#include "../Common/EntityByVertexTypeManager.hpp"
#include "../Scene/Resources/Model.hpp"

namespace GraphicEngine::Services
{
	using ModelEntityContainer = Common::EntityByVertexTypeManager<Scene::Model>;
	class ModelManager
	{
	public:
		template <typename VertexType>
		void addModel(std::shared_ptr<Scene::Model<VertexType>> model)
		{
			m_modelContainer->addEntity(model);
		}

		std::shared_ptr<ModelEntityContainer> getModelEntityContainer();
	protected:
	private:
		std::shared_ptr<ModelEntityContainer> m_modelContainer = std::make_shared<ModelEntityContainer>();;
	};
}