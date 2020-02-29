#ifndef GRAPHIC_ENGINE_CORE_SHADER_HPP
#define GRAPHIC_ENGINE_CORE_SHADER_HPP

#include <fstream>

namespace GraphicEngine
{
	class Shader
	{
	public:
		Shader() {}

		Shader(const std::string& code)
		{
			_data = code;
		}

		template <typename Reader>
		Shader(Reader reader, const std::string& path)
		{
			_data = reader(path);
		}

		virtual void compile() = 0;
	protected:
		std::string _data;
	};
}

#endif // !GRAPHIC_ENGINE_CORE_SHADER_HPP

