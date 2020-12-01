#include "ModelManager.hpp"

std::shared_ptr<GraphicEngine::Services::ModelEntityContainer> GraphicEngine::Services::ModelManager::getModelEntityContainer()
{
    return m_modelContainer;
}
