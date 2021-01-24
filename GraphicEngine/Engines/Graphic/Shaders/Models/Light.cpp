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
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f); //glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(7.5f * glm::vec3(-glm::normalize(direction)), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 lightProjection = glm::ortho(-10.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
	//glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
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
}

GraphicEngine::Engines::Graphic::Shaders::SpotLight::SpotLight(glm::vec4 position, glm::vec4 direction, float innerCutOff, float outterCutOff, float constant, float linear, float quadric, LightColor color) :
	position{ position }, direction{ direction }, innerCutOff{ innerCutOff }, outterCutOff{ outterCutOff }, constant{ constant }, linear{ linear }, quadric{ quadric }, color{ color } {}
