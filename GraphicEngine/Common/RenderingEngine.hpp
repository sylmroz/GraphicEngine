#pragma once

#define NOMINMAX

#include "../Core/EventManager.hpp"
#include "../Core/Logger.hpp"
#include "Vertex.hpp"
#include "Camera.hpp"
#include "../Scene/Resources/Model.hpp"
#include "../Modules/Assimp/AssimpModelImporter.hpp"
#include "../Core/Utils/ObjectConverter.hpp"
#include "../Services/CameraControllerManager.hpp"
#include "../Services/ModelManager.hpp"

#include "../Engines/Graphic/3D/ObjectGenerator.hpp"
#include "../Engines//Graphic/Shaders/Models/Light.hpp"

namespace GraphicEngine
{
	class RenderingEngine
	{
	public:
		RenderingEngine(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
			std::shared_ptr<Services::ModelManager> modelManager,
			std::shared_ptr<Core::EventManager> eventManager,
			std::shared_ptr<Core::Configuration> cfg) :
			m_cameraControllerManager{ cameraControllerManager },
			m_modelManager{ modelManager },
			m_eventManager{ eventManager },
			m_cfg{ cfg }
		{
			/*auto cylinder = Engines::Graphic::ConeGenerator<Common::VertexPNW>{}.getModel(glm::vec3(0.0f), 0.5f, 2.0f, glm::ivec3(20, 2, 2), false, true, Engines::Graphic::TriangleDirection::CounterClockwise);
			m_modelManager->addModel(cylinder);*/
			// m_models.push_back(cylinder);
			/*m_models.front()->setScale(m_cfg->getProperty<float>("scene:object:scale"));
			m_models.front()->setRotate(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:rotate")));
			m_models.front()->setPosition(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:position")));

			m_models.front()->getMeshes().front()->setRotate(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(m_cfg->getProperty<std::vector<float>>("scene:object:rotate")));*/
			auto cube = Engines::Graphic::CuboidGenerator<Common::VertexPN>{}.getModel(glm::vec3(-0.25f), glm::vec3(0.25), glm::ivec3(3), Engines::Graphic::GeneratingPosition::Corner, Engines::Graphic::TriangleDirection::CounterClockwise);
			m_modelManager->addModel(cube);

			/*auto sphere = Engines::Graphic::SphereGenerator<Common::VertexPN>{}.getModel(glm::vec3(0.0f), 0.25, glm::ivec2(20), Engines::Graphic::TriangleDirection::CounterClockwise);
			m_modelManager->addModel(sphere);*/
		}

		virtual bool drawFrame() = 0;
		virtual void init(size_t width, size_t height) = 0;
		virtual void resizeFrameBuffer(size_t width, size_t height) = 0;
		virtual void cleanup() = 0;

		virtual ~RenderingEngine() = default;
	protected:
		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<Services::ModelManager> m_modelManager;
		std::shared_ptr<Core::EventManager> m_eventManager;
		std::shared_ptr<Core::Configuration> m_cfg;

		glm::vec4 backgroudColor{ 0.2f, 0.2f, 0.2f, 1.0f };
	};
}
