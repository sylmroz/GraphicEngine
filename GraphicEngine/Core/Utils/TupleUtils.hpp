#pragma once

#include <tuple>
#include <type_traits>

namespace GraphicEngine::Core::Utils
{
	template <typename... Types>
	struct RegisterTypes
	{
		static const std::tuple<Types...> types = { Types{}... };
	};

	template <std::size_t I = 0, typename FuncT, typename... Tp>
	inline typename std::enable_if<I == sizeof...(Tp), void>::type
		for_each(std::tuple<Tp...>&, FuncT)
	{}

	template <std::size_t I = 0, typename FuncT, typename... Tp>
	inline typename std::enable_if < I < sizeof...(Tp), void>::type
		for_each(std::tuple<Tp...>& t, FuncT f)
	{
		f(std::get<I>(t));
		for_each<I + 1, FuncT, Tp...>(t, f);
	}
}