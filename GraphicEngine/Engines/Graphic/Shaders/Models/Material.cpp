#include "Material.hpp"
#include "../../../../Core/Utils/ObjectConverter.hpp"

GraphicEngine::Engines::Graphic::Shaders::Material::Material(std::shared_ptr<Core::Configuration> cfg)
{
	ambient = Core::Utils::Converter::fromArrayToObject<glm::vec4, std::vector<float>, 4>(cfg->getProperty<std::vector<float>>("ambient"));
	diffuse = Core::Utils::Converter::fromArrayToObject<glm::vec4, std::vector<float>, 4>(cfg->getProperty<std::vector<float>>("diffuse"));
	specular = Core::Utils::Converter::fromArrayToObject<glm::vec4, std::vector<float>, 4>(cfg->getProperty<std::vector<float>>("specular"));
	shininess = cfg->getProperty<float>("shininess");
}