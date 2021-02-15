#include "TokenRepleacer.hpp"

std::string GraphicEngine::Core::Utils::repleaceEachOccurence(const std::string& text, const std::string& key, const std::string& value)
{
	std::string tmp = text;
	while (true)
	{
		const std::size_t pos = tmp.find(key);
		if (pos == std::string::npos)
			return tmp;
		tmp = tmp.replace(pos, key.length(), value);
	}
}

std::string GraphicEngine::Core::Utils::tokenRepleacer(const std::string& text, const std::map<std::string, std::string>& keyValues)
{
	std::string tmp = text;
	for (auto& [key, value] : keyValues)
	{
		tmp = repleaceEachOccurence(tmp, key, value);
	}
	return tmp;
}
