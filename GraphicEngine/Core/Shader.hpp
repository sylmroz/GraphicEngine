#ifndef GRAPHIC_ENGINE_CORE_SHADER_HPP
#define GRAPHIC_ENGINE_CORE_SHADER_HPP

#include <fstream>
#include <string>

template <typename _Lang>
class Shader
{
public:
	Shader(const std::string& path)
	{
		_type = type;

		std::ifstream shaderFile(path.c_str(), std::ios::binary|std::ios::ate);

		if (!shaderFile.is_open())
			throw std::runtime_error("Failed when open shader file: " + path);

		auto fileSize = shaderFile.tellg();
		shaderFile.seekg(0);
		_data.reserve(fileSize);
		shaderFile.read(_data.data(), _data.size());
		shaderFile.close();
	}

	void compile()
	{
		try
		{
			static_cast<_Lang*>(this)->_compile();
		}

		catch (std::runtime_error err)
		{
			throw err;
		}
	}
protected:
	std::string _data;
};

#endif // !GRAPHIC_ENGINE_CORE_SHADER_HPP

