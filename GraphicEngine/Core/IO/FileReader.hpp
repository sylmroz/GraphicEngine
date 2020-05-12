#pragma once

#include <fstream>

namespace GraphicEngine::Core::IO
{
	template <typename String>
	String readFile(const std::string& path)
	{
		String _data;
		std::ifstream shaderFile(path.c_str(), std::ios::binary | std::ios::ate);

		if (!shaderFile.is_open())
			throw std::runtime_error("Failed when open file: " + path);

		auto fileSize = shaderFile.tellg();
		shaderFile.seekg(0, std::ios::beg);
		_data.reserve(fileSize);
		_data.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());

		shaderFile.close();

		return _data;
	}
}
