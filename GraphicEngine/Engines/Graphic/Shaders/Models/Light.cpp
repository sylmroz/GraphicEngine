#include "Light.hpp"

GraphicEngine::Engines::Graphic::Shaders::LightColor::LightColor(std::shared_ptr<Core::Configuration> cfg)
{
	diffuse = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("diffuse")), 1.0f);
	ambient = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("ambient")), 1.0f);
	specular = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("specular")), 1.0f);
}

GraphicEngine::Engines::Graphic::Shaders::LightColor::LightColor(glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 specular) :
	diffuse{ diffuse }, ambient{ ambient }, specular{ specular } {}

GraphicEngine::Engines::Graphic::Shaders::DirectionalLight::DirectionalLight()
{
	calculateLigthSpace();
}

GraphicEngine::Engines::Graphic::Shaders::DirectionalLight::DirectionalLight(std::shared_ptr<Core::Configuration> cfg)
{
	direction = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("direction")), 1.0f);
	calculateLigthSpace();
	// Need to be done in this way - normally throw error
	auto props = cfg->getProperty<json>("color");
	props.items();
	color = LightColor(std::make_shared<Core::Configuration>(props));
}

GraphicEngine::Engines::Graphic::Shaders::DirectionalLight::DirectionalLight(glm::vec4 direction, LightColor color) :
	direction{ direction }, color{ color }
{
	calculateLigthSpace();
}

void GraphicEngine::Engines::Graphic::Shaders::DirectionalLight::calculateLigthSpace()
{
	float far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f);
	glm::mat4 lightView = glm::lookAt(far_plane * glm::vec3(-glm::normalize(direction)), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpace = lightProjection * lightView;
}

GraphicEngine::Engines::Graphic::Shaders::PointLight::PointLight(std::shared_ptr<Core::Configuration> cfg)
{
	position = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("position")), 1.0f);
	constant = cfg->getProperty<float>("constant");
	linear = cfg->getProperty<float>("linear");
	quadric = cfg->getProperty<float>("quadric");
	auto props = cfg->getProperty<json>("color");
	props.items();
	color = LightColor(std::make_shared<Core::Configuration>(props));
}

GraphicEngine::Engines::Graphic::Shaders::PointLight::PointLight(glm::vec4 position, float constant, float linear, float quadric, LightColor color) :
	position{ position }, constant{ constant }, linear{ linear }, quadric{ quadric }, color{ color } {}

std::array<GraphicEngine::Engines::Graphic::Shaders::LightSpaceMatrix, 6> GraphicEngine::Engines::Graphic::Shaders::PointLight::getLightSpaceMatrices()
{
	std::array<Engines::Graphic::Shaders::LightSpaceMatrix, 6> lightSpaceMatrices;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.0f, 25.0f);
	lightSpaceMatrices[0] = shadowProj * glm::lookAt(glm::vec3(position), glm::vec3(position) + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	lightSpaceMatrices[1] = shadowProj * glm::lookAt(glm::vec3(position), glm::vec3(position) + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	lightSpaceMatrices[2] = shadowProj * glm::lookAt(glm::vec3(position), glm::vec3(position) + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	lightSpaceMatrices[3] = shadowProj * glm::lookAt(glm::vec3(position), glm::vec3(position) + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	lightSpaceMatrices[4] = shadowProj * glm::lookAt(glm::vec3(position), glm::vec3(position) + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	lightSpaceMatrices[5] = shadowProj * glm::lookAt(glm::vec3(position), glm::vec3(position) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	return lightSpaceMatrices;
}

GraphicEngine::Engines::Graphic::Shaders::SpotLight::SpotLight()
{
	calculateLigthSpace();
}

GraphicEngine::Engines::Graphic::Shaders::SpotLight::SpotLight(std::shared_ptr<Core::Configuration> cfg)
{
	position = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("position")), 1.0f);
	direction = glm::vec4(Core::Utils::Converter::fromArrayToObject<glm::vec3, std::vector<float>, 3>(cfg->getProperty<std::vector<float>>("direction")), 1.0f);
	innerCutOff = glm::cos(glm::radians(cfg->getProperty<float>("inner cutoff")));
	outterCutOff = glm::cos(glm::radians(cfg->getProperty<float>("outter cutoff")));
	constant = cfg->getProperty<float>("constant");
	linear = cfg->getProperty<float>("linear");
	quadric = cfg->getProperty<float>("quadric");
	auto props = cfg->getProperty<json>("color");
	props.items();
	color = LightColor(std::make_shared<Core::Configuration>(props));
	calculateLigthSpace();
}

GraphicEngine::Engines::Graphic::Shaders::SpotLight::SpotLight(glm::vec4 position, glm::vec4 direction, float innerCutOff, float outterCutOff, float constant, float linear, float quadric, LightColor color) :
	position{ position }, direction{ direction }, innerCutOff{ innerCutOff }, outterCutOff{ outterCutOff }, constant{ constant }, linear{ linear }, quadric{ quadric }, color{ color } 
{
	calculateLigthSpace();
}

void GraphicEngine::Engines::Graphic::Shaders::SpotLight::calculateLigthSpace()
{
	glm::mat4 lightProjection = glm::perspective(2*glm::radians(glm::degrees(glm::acos(outterCutOff))), 1.0f, 2.0f, 50.0f);
	glm::mat4 lightView = glm::lookAt(glm::vec3(position), glm::vec3(position) + glm::normalize(glm::vec3(direction)), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpace = lightProjection * lightView;
}
