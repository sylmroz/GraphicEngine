#pragma once

#include "../Core/Input/Keyboard/KeyboardEnumKeys.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace GraphicEngine::Common
{
	class Keyboard
	{
	public:
		virtual ~Keyboard() = default;
		virtual std::vector<Core::Inputs::KeyboardKey> getPressedKeys() = 0;
		
	protected:
		std::vector<Core::Inputs::KeyboardKey> _keys;
	};
}
