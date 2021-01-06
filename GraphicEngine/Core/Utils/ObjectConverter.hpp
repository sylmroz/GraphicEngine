#pragma once

namespace GraphicEngine::Core::Utils::Converter
{
	template <typename Obj, typename Array, int N>
	Obj fromArrayToObject(Array arr)
	{
		if constexpr (N == 2)
		{
			return Obj(arr[0], arr[1]);
		}

		if constexpr (N == 3)
		{
			return Obj(arr[0], arr[1], arr[2]);
		}

		if constexpr (N == 4)
		{
			return Obj(arr[0], arr[1], arr[2], arr[3]);
		}
	}
}
