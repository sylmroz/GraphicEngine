#pragma once

#include <string>

namespace GraphicEngine
{
	class Shader
	{
	public:
		Shader() {}

		Shader(const std::string& code)
		{
			m_data = code;
		}

		template <typename Reader, std::enable_if_t<std::is_function_v<Reader>, int> = 0>
		Shader(Reader reader, const std::string& path)
		{
			m_data = reader(path);
		}

		virtual void compile() = 0;
	protected:
		std::string m_data;
	};
}
