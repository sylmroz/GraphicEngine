#include "ViewportManager.hpp"

GraphicEngine::Services::ViewportManager::ViewportManager(std::shared_ptr<Core::Configuration> cfg)
{
	try {
		displayWireframe = cfg->getProperty<bool>("viewport options:wireframe");
		displayNormal = cfg->getProperty<bool>("viewport options:normals");
		displaySolid = cfg->getProperty<bool>("viewport options:solid");
		displaySkybox = cfg->getProperty<bool>("viewport options:skybox");
	}

	catch (std::runtime_error err)
	{

	}

	catch (...)
	{

	}
}
