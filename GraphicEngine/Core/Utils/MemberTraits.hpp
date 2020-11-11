#pragma once
#include <type_traits>

namespace GraphicEngine::Core::Utils
{
	// Meta structures to check is used type has particular member
	template <typename, typename = void>
	struct has_position_member : std::false_type {};

	template <typename Vertex>
	struct has_position_member <Vertex, std::void_t<decltype(std::declval<Vertex>().position)>> : std::true_type {};

	template <typename, typename = void>
	struct has_normal_member : std::false_type {};

	template <typename Vertex>
	struct has_normal_member <Vertex, std::void_t<decltype(std::declval<Vertex>().normal)>> : std::true_type {};

	/*template <typename Vertex>
	using has_normal_member_v = has_normal_member<Vertex>::value;*/

	template <typename, typename = void>
	struct has_color_member : std::false_type {};

	template <typename Vertex>
	struct has_color_member <Vertex, std::void_t<decltype(std::declval<Vertex>().color)>> : std::true_type {};

	/*template <typename Vertex>
	using has_color_member_v = has_color_member<Vertex>::value;*/

	template <typename, typename = void>
	struct has_texCoord_member : std::false_type {};

	template <typename Vertex>
	struct has_texCoord_member <Vertex, std::void_t<decltype(std::declval<Vertex>().texCoord)>> : std::true_type {};

	/*template <typename Vertex>
	using has_texCoord_member_v = has_texCoord_member<Vertex>::value;*/

	template <typename, typename = void>
	struct has_tangent_member : std::false_type {};

	template <typename Vertex>
	struct has_tangent_member <Vertex, std::void_t<decltype(std::declval<Vertex>().tangent)>> : std::true_type {};

	/*template <typename Vertex>
	using has_tangent_member_v = has_tangent_member<Vertex>::value;*/

	template <typename, typename = void>
	struct has_bitangent_member : std::false_type {};

	template <typename Vertex>
	struct has_bitangent_member <Vertex, std::void_t<decltype(std::declval<Vertex>().bitangent)>> : std::true_type {};

	/*template <typename Vertex>
	using has_bitangent_member_v = has_bitangent_member<Vertex>::value;*/

	template <typename, typename = void>
	struct has_weight_member : std::false_type {};

	template <typename Vertex>
	struct has_weight_member <Vertex, std::void_t<decltype(std::declval<Vertex>().weight)>> : std::true_type {};

	/*template <typename Vertex>
	using has_weight_member_v = has_weight_member<Vertex>::value;*/
}