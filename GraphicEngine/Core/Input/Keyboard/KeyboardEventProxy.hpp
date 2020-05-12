#pragma once

#include "KeyboardEnumKeys.hpp"

#include "../../Subject.hpp"

#include <vector>

namespace GraphicEngine::Core::Inputs
{
	class KeyboardEventProxy: public Subject<std::vector<KeyboardKey>>
	{
	};
}
