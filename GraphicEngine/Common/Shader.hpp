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

		template <typename Reader>
		Shader(Reader reader, const std::string& path)
		{
			m_data = reader(path);
		}

		virtual void compile() = 0;
	protected:
		std::string m_data;
	};
}
