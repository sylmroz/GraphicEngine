#pragma once

#include <nlohmann/json.hpp>
#include "../../../UtilityLib/UtilityLib/Utility/tokenizer.hpp"

using json = nlohmann::json;

namespace GraphicEngine::Core
{
	class Configuration
	{
	public:
		Configuration(const std::string& fileName = "AppSettings.json");
		Configuration(json& j);

		template <typename T>
		T getProperty(const std::string& key)
		{
			std::vector<std::string> allKeys = UtilityLib::tokenizer::tokenize<std::vector<std::string>>(std::begin(key), std::end(key), ":");
			return getProperty<T>(configuration, allKeys, 0);
		}

	private:

		template <typename T>
		T getProperty(const json& j, const std::vector<std::string>& keys, int index)
		{
			if (index >= keys.size())
			{
				throw std::runtime_error("Property key do not exist!");
			}

			auto& props = j[keys[index]];
			if (props.is_null())
			{
				std::string err = "Property " + keys[index] + " do not exist!";
				throw std::runtime_error(err);
			}

			if (index == keys.size() - 1)
			{
				return props;
			}

			else
			{
				return getProperty<T>(props, keys, index + 1);
			}
		}

		json configuration;
	};
}
