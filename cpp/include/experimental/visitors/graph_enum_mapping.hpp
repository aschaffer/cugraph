#pragma once

#include <experimental/graph.hpp>
#include "graph_enum.hpp"

namespace cugraph {
namespace experimental {

template <typename vertex_t,
          typename edge_t,
          typename weight_t,
          bool,
          bool,
          GTypes>
struct GMapType;  // primary template, purposely empty

// partial specializations:
//
template <typename vertex_t, typename edge_t, typename weight_t, bool st_tr, bool multi_gpu>
struct GMapType<vertex_t, edge_t, weight_t, st_tr, multi_gpu, GTypes::GRAPH_T> {
  using type = graph_t<vertex_t, edge_t, weight_t, st_tr, multi_gpu>;
};

}  // namespace experimental
}  // namespace cugraph
