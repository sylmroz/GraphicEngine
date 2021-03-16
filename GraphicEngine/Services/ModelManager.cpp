#include "ModelManager.hpp"
#include <magic_enum.hpp>
#include "../Engines/Graphic/3D/ObjectGenerator.hpp"
#include "../Modules/Assimp/AssimpModelImporter.hpp"

enum class ObjectSourceType
{
    Generator = 1,
    File = 2,
};

GraphicEngine::Services::ModelManager::ModelManager(std::shared_ptr<Core::Configuration> cfg, std::unique_ptr<Core::Logger<ModelManager>> logger):
    m_logger{ std::move(logger) }
{
    try
    {
        auto objectsDefinitions = cfg->getProperty<std::vector<json>>("scene:objects");
        for (auto objectDefinition : objectsDefinitions)
        {
            m_logger->info(__FILE__, __LINE__, __FUNCTION__, "Read model properties {}", objectDefinition.dump());
            Core::Configuration tmpConfiguration(objectDefinition);
            auto typeStr = tmpConfiguration.getProperty<std::string>("type");
            auto typeEnum = magic_enum::enum_cast<ObjectSourceType>(typeStr);
            if (typeEnum.value() == ObjectSourceType::Generator)
            {
                // TODO detect type of vertex from enum
                auto j = tmpConfiguration.getProperty<json>("model");
                auto modelConfiguration = std::make_shared<Core::Configuration>(j);
                auto type = magic_enum::enum_cast<Engines::Graphic::GeneratorType>(modelConfiguration->getProperty<std::string>("type"));
                auto j2 = modelConfiguration->getProperty<json>("model");
                auto generatorModelProperties = std::make_shared<Core::Configuration>(j2);
                auto model = Engines::Graphic::ObjectGenerator<Common::VertexPN>::generateModel(generatorModelProperties, type.value());
                model->setScale(generatorModelProperties->getProperty<float>("transoformation:scale"));
                model->setRotate(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(generatorModelProperties->getProperty<std::vector<float>>("transoformation:rotate")));
                model->setPosition(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(generatorModelProperties->getProperty<std::vector<float>>("transoformation:position")));
                addModel(model);
            }
            if (typeEnum.value() == ObjectSourceType::File)
            {
                auto j = tmpConfiguration.getProperty<json>("model");
                auto modelConfiguration = std::make_shared<Core::Configuration>(j);
                auto models = Modules::AssimpModelImporter<Common::VertexPN>{}.read(modelConfiguration->getProperty<std::string>("path"));
                models.front()->setScale(modelConfiguration->getProperty<float>("transoformation:scale"));
                models.front()->setRotate(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(modelConfiguration->getProperty<std::vector<float>>("transoformation:rotate")));
                models.front()->setPosition(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(modelConfiguration->getProperty<std::vector<float>>("transoformation:position")));
                models.front()->applyTransformation();
                for (auto model : models)
                    addModel(model);
            }
        }
    }
    // TODO more catchs
    catch (...)
    {

    }
}

std::shared_ptr<GraphicEngine::Services::ModelEntityContainer> GraphicEngine::Services::ModelManager::getModelEntityContainer()
{
    return m_modelContainer;
}
