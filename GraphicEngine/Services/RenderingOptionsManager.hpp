#pragma once

#include "../Engines/Graphic/Shaders/Models/RenderingOptions.hpp"
#include "../Core/Configuration.hpp"
#include "../Core/Subject.hpp"
#include <glm/vec4.hpp>

namespace GraphicEngine::Services
{
	class RenderingOptionsManager
	{
	public:
		RenderingOptionsManager(std::shared_ptr<Core::Configuration> cfg);

		Engines::Graphic::Shaders::RenderingOptions renderingOptions;

		void onUpdateOptions(std::function<void(Engines::Graphic::Shaders::RenderingOptions)> callback);

		void setRenderingOptions(Engines::Graphic::Shaders::RenderingOptions renderingOptions);

	private:
		Core::Subject<Engines::Graphic::Shaders::RenderingOptions> m_updateOptionsSubject;
	};
}
