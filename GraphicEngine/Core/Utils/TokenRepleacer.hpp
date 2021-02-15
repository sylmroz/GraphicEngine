#pragma once

#include <map>
#include <string>

namespace GraphicEngine::Core::Utils
{
	std::string repleaceEachOccurence(const std::string& text, const std::string& key, const std::string& value);
	std::string tokenRepleacer(const std::string& text, const std::map<std::string, std::string>& keyValues);
}