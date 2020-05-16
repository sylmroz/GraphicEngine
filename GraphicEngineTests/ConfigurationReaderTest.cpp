#include "pch.h"
#include "../GraphicEngine/Core/Configuration.hpp"

using namespace GraphicEngine::Core;

using json = nlohmann::json;

json j = R"(
{
  "engine": "opengl",
  "window": {
    "type": "glfw",
    "allow resize": true,
    "fullscreen": false,
    "width": 640,
    "height": 480
  },
  "debug": {
    "level": "info"
  }
}
)"_json;

TEST(ConfigurationPropertyGetterTest, IsBasicValuesCorrect)
{
    Configuration configuration(j);
    std::string engineType = configuration.getProperty<std::string>("engine");
    EXPECT_EQ(engineType, "opengl");
}

TEST(ConfigurationPropertyGetterTest, IsComoundValueParseCorrectly)
{
    Configuration configuration(j);
    int engineType = configuration.getProperty<int>("window:width");
    EXPECT_EQ(engineType, 640);
}