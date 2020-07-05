#include "Mouse.hpp"

GraphicEngine::Common::Mouse::Mouse(std::shared_ptr<Core::Configuration> cfg):
	m_cfg{cfg}
{
	m_sensitivity = m_cfg->getProperty<float>("mouse:sensitivity");
}

float GraphicEngine::Common::Mouse::getSensitivity()
{
	return m_sensitivity;
}

void GraphicEngine::Common::Mouse::setSensitivity(float sensitivity)
{
	m_sensitivity = sensitivity;
}
