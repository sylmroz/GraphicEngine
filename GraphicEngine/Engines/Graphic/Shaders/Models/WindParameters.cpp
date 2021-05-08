#include "WindParameters.hpp"
#include "../../../../Core/Utils/ObjectConverter.hpp"

GraphicEngine::Engines::Graphic::Shaders::WindParameters::WindParameters(std::shared_ptr<Core::Configuration> cfg)
{
	direcion = Core::Utils::Converter::fromArrayToObject<glm::vec2, std::vector<float>, 2>(cfg->getProperty<std::vector<float>>("direction"));
	auto speedValue = cfg->getProperty<float>("speed");
	speed = std::min(std::max(speedValue, 0.0f), 1.0f);
}