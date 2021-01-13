#pragma once

#include "../Core/Configuration.hpp"
#include <glm/vec4.hpp>

namespace GraphicEngine::Services
{
	class ViewportManager
	{
	public:
		ViewportManager(std::shared_ptr<Core::Configuration> cfg);

		glm::vec4 backgroudColor{ 0.2f, 0.2f, 0.2f, 1.0f };

		bool displayWireframe{ true };
		bool displaySolid{ true };
		bool displayNormal{ true };
		bool displaySkybox{ true };
	};
}
