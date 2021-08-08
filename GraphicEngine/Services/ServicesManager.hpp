#pragma once

#include <any>
#include <map>
#include <memory>

#include "../Core/Utils/GetClassName.hpp"


namespace GraphicEngine::Services
{
	class CameraControllerManager;
	class ModelManager;
	class ViewportManager;
	class RenderingOptionsManager;
	class WindManager;
	class LightManager;

	class ServicesManager
	{
	public:
		ServicesManager(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager,
			std::shared_ptr<Services::ModelManager> modelManager,
			std::shared_ptr<Services::LightManager> lightManager,
			std::shared_ptr<Services::ViewportManager> viewportManager,
			std::shared_ptr<Services::RenderingOptionsManager> renderingOptionsManager,
			std::shared_ptr<Services::WindManager> windManager);

		template<typename T>
		std::shared_ptr<T> addService(std::shared_ptr<T> service)
		{
			auto className = Core::Utils::getFullClassName<T>();
			auto it = m_services.find(className);
			if (it == std::end(m_services))
			{
				m_services[className] = service;
			}
			return service;
		}

		template<typename T>
		std::shared_ptr<T> getService()
		{
			auto className = Core::Utils::getFullClassName<T>();
			auto it = m_services.find(className);
			if (it == std::end(m_services))
			{
				return nullptr;
			}
			return std::any_cast<std::shared_ptr<T>>(m_services[className]);
		}
	private:
		std::map<std::string, std::any> m_services;
	};
}