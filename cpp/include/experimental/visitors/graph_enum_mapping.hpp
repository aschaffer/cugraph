#pragma once

#include <experimental/graph.hpp>
#include "graph_enum.hpp"

namespace cugraph {
namespace experimental {

template <typename vertex_t,
          typename edge_t,
          typename weight_t,
          bool store_transposed,
          bool multi_gpu,
          GTypes>
struct GMapType;  // primary template, purposely empty

// partial specializations:
//
template <typename vertex_t,
          typename edge_t,
          typename weight_t,
          bool store_transposed,
          bool multi_gpu>
struct GMapType<vertex_t, edge_t, weight_t, store_transposed, multi_gpu, GTypes::GRAPH_T> {
  using type = graph_t<vertex_t, edge_t, weight_t, store_transposed, multi_gpu>;
};

template <typename vertex_t,
          typename edge_t,
          typename weight_t,
          bool store_transposed,
          bool multi_gpu>
struct GMapType<vertex_t, edge_t, weight_t, store_transposed, multi_gpu, GTypes::GRAPH_VIEW> {
  using type = graph_view_t<vertex_t, edge_t, weight_t, store_transposed, multi_gpu>;
};

}  // namespace experimental
}  // namespace cugraph
