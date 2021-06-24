#pragma once

#include "../Engines/Graphic/Shaders/Models/WindParameters.hpp"
#include "../Core/Subject.hpp"

#include "../Engines/Graphic/2D/WindGenerator.hpp"
#include <any>

namespace GraphicEngine::Services
{
	class WindManager
	{
	public:
		WindManager(std::shared_ptr<Core::Configuration> cfg);

		Engines::Graphic::Shaders::WindParameters getWindParameters();

		void onUpdateWindParameters(std::function<void(Engines::Graphic::Shaders::WindParameters)> callback);

		void setWindParameters(Engines::Graphic::Shaders::WindParameters windParameters);

		void setWindSpeed(float speed);
		void setWindDirection(glm::vec2 direction);

		void generateWindTexture(uint32_t resolution = 1024, float speed = 1.0);

		uint32_t getWindTextureResolution();

		template <typename Texture>
		std::shared_ptr<Texture> getTextureObject()
		{
			if (!m_textureObject.has_value())
				m_textureObject = std::make_shared<Texture>(m_windTexture.data, m_resolution, m_resolution, 4, false);
			return std::any_cast<std::shared_ptr<Texture>>(m_textureObject);
		}

	private:
		void loadWind();
	private:
		Engines::Graphic::Shaders::WindParameters m_windParameters{};
		Core::Subject<Engines::Graphic::Shaders::WindParameters> m_updateWindParametersSubject;

		cv::Mat m_windTexture;
		std::string m_windTexturePath;

		uint32_t m_resolution;
		std::any m_textureObject;
	};
}
