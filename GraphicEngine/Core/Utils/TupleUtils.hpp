#pragma once

#include <tuple>
#include <type_traits>

namespace GraphicEngine::Core::Utils
{
	template <typename... Types>
	struct TypesRegister
	{
		static const std::tuple<Types...> types;
	};

	template <typename... Types>
	const std::tuple<Types...> TypesRegister<Types...>::types = { Types{}... };

	template <std::size_t I = 0, typename FuncT, typename... Tp>
	inline typename std::enable_if<I == sizeof...(Tp), void>::type
		for_each(std::tuple<Tp...>&, FuncT)
	{}

	template <std::size_t I = 0, typename FuncT, typename... Tp>
	inline typename std::enable_if < I < sizeof...(Tp), void>::type
		for_each(std::tuple<Tp...>& t, FuncT f)
	{
		f(std::get<I>(t));
		for_each<I + 1, FuncT, Tp..., void>(t, f);
	}

	template <std::size_t I = 0, typename FuncT, typename... Tp>
	inline typename std::enable_if<I == sizeof...(Tp), void>::type
		for_each(const std::tuple<Tp...>&, FuncT)
	{}

	template <std::size_t I = 0, typename FuncT, typename... Tp>
	inline typename std::enable_if < I < sizeof...(Tp), void>::type
		for_each(const std::tuple<Tp...>& t, FuncT f)
	{
		f(std::get<I>(t));
		for_each<I + 1, FuncT, Tp...>(t, f);
	}
}