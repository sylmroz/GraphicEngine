#pragma once

#define NOMINMAX

#include "../Core/EventManager.hpp"
#include "../Core/Logger.hpp"
#include "Vertex.hpp"
#include "Camera.hpp"
#include "UI.hpp"
#include "../Scene/Resources/Model.hpp"
#include "../Modules/Assimp/AssimpModelImporter.hpp"
#include "../Core/Utils/ObjectConverter.hpp"
#include "../Services/CameraControllerManager.hpp"
#include "../Services/ModelManager.hpp"
#include "../Services/LightManager.hpp"
#include "../Services/ViewportManager.hpp"
#include "../Services/RenderingOptionsManager.hpp"
#include "../Services/WindManager.hpp"

#include "../Engines//Graphic/Shaders/Models/Light.hpp"

#include "../UI/ImGui/Widgets/ColorEdit.hpp"
#include "../UI/ImGui/Widgets/Checkbox.hpp"

namespace GraphicEngine
{
	class RenderingEngine
	{
	public:
		RenderingEngine(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
			std::shared_ptr<Services::ModelManager> modelManager,
			std::shared_ptr<Services::LightManager> lightManager,
			std::shared_ptr<Services::ViewportManager> viewportManager,
			std::shared_ptr<Services::RenderingOptionsManager> renderingOptionsManager,
			std::shared_ptr<Services::WindManager> windManager,
			std::shared_ptr<Core::EventManager> eventManager,
			std::shared_ptr<Core::Timer> timer,
			std::shared_ptr<Common::UI> ui,
			std::shared_ptr<Core::Configuration> cfg);

		virtual bool drawFrame() = 0;
		virtual void init(size_t width, size_t height) = 0;
		virtual void resizeFrameBuffer(size_t width, size_t height) = 0;
		virtual void cleanup() = 0;

		virtual ~RenderingEngine() = default;
	protected:
		std::shared_ptr<Services::CameraControllerManager> m_cameraControllerManager;
		std::shared_ptr<Services::ModelManager> m_modelManager;
		std::shared_ptr<Services::LightManager> m_lightManager;
		std::shared_ptr<Core::EventManager> m_eventManager;
		std::shared_ptr<Core::Timer> m_timer;
		std::shared_ptr<Common::UI> m_ui;
		std::shared_ptr<Core::Configuration> m_cfg;
		std::shared_ptr<Services::ViewportManager> m_viewportManager;
		std::shared_ptr<Services::RenderingOptionsManager> m_renderingOptionsManager;
		std::shared_ptr<Services::WindManager> m_windManager;
	};
}
