#include "CameraManagerWindow.hpp"
#include "../Widgets/ComboBox.hpp"

GraphicEngine::GUI::CameraManagerWindow::CameraManagerWindow(std::shared_ptr<Services::CameraControllerManager> cameraControllerManager) :
	m_cameraControllerManager{ cameraControllerManager }
{
	m_container = std::make_shared<CollapsingHeader>("Cameras manager");
	std::vector<std::string> labels;
	for (uint32_t i{ 0 }; i < m_cameraControllerManager->getCameraControllers().size(); ++i)
	{
		auto lab = std::string(std::string("Camera ") + std::to_string(i));
		labels.push_back(lab);
	}
	auto cameraSelector = std::make_shared<ComboBox>("Select Camera", std::move(labels), m_cameraControllerManager->getActiveCameraIndex() - 1);
	cameraSelector->onItemSelected([&](int index)
	{
		m_cameraControllerManager->activateCameraController(index + 1);
	});
	m_container->addChildren(cameraSelector);
}

void GraphicEngine::GUI::CameraManagerWindow::draw()
{
	m_container->draw();
}
