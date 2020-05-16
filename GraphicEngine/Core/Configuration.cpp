#include "Configuration.hpp"
#include <fstream>

GraphicEngine::Core::Configuration::Configuration(const std::string& fileName)
{
	std::fstream i(fileName);
	if (i.is_open())
	{
		try
		{
			i >> configuration;
		}

		catch (json::exception ex)
		{
			throw std::runtime_error(ex.what());
		}
	}

	else
	{
		std::string err = "File  " + fileName + " not found!";
		throw std::runtime_error(err);
	}
}

GraphicEngine::Core::Configuration::Configuration(const json& j):
	configuration{j}
{
}
