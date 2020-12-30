#pragma once

#include <type_traits>

namespace cugraph {
namespace experimental {

// primary template:
//
template <typename Src, typename... Types>
struct is_one_of;  // purposely empty

// partial specializations:
//
template <typename Src, typename Head, typename... Tail>
struct is_one_of<Src, Head, Tail...> {
  static constexpr bool value = std::is_same<Src, Head>::value || is_one_of<Src, Tail...>::value;
};

template <typename Src>
struct is_one_of<Src> {
  static constexpr bool value = false;
};

// define template param candidates:
//
template <typename vertex_t, typename edge_t, typename weight_t>
struct is_candidate {
  static constexpr bool value =
    is_one_of<vertex_t, int32_t, int64_t>::value && is_one_of<edge_t, int32_t, int64_t>::value &&
    (sizeof(vertex_t) <= sizeof(edge_t)) && is_one_of<weight_t, float, double>::value;
};

}  // namespace experimental
}  // namespace cugraph
