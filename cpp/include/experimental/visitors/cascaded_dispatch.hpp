///#pragma once
// get an error if included multiple places,
// to make sure it's only included in one place

#include <array>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "enum_mapping.hpp"
#include "graph_enum_mapping.hpp"

namespace cugraph {
namespace experimental {

// final step of cascading: calls f():
//
template <typename vertex_t,
          typename edge_t,
          typename weight_t,
          bool tr,
          bool mg,
          typename FType,
          typename... Fargs>
constexpr decltype(auto) graph_dispatcher(GTypes graph_type, FType f, Fargs&&... args)
{
  switch (graph_type) {
    case GTypes::GRAPH_T: {
      using graph_t = typename GMapType<vertex_t, edge_t, weight_t, tr, mg, GTypes::GRAPH_T>::type;
      return f.template operator()<graph_t>(std::forward<Fargs>(args)...);
    } break;

    default: {
      std::stringstream ss;
      ss << "ERROR: Unknown type enum:" << static_cast<int>(graph_type);
      throw std::runtime_error(ss.str());
    }
  }
}

// multi_gpu bool dispatcher:
// resolves bool `multi_gpu`
// and using template arguments vertex_t, edge_t, weight_t, store_transpose
// cascades into next level
// graph_dispatcher()
//
template <typename vertex_t,
          typename edge_t,
          typename weight_t,
          bool store_transposed,
          typename FType,
          typename... Fargs>
constexpr decltype(auto) multi_gpu_dispatcher(bool multi_gpu,
                                              GTypes graph_type,
                                              FType f,
                                              Fargs&&... args)
{
  switch (multi_gpu) {
    case true: {
      return graph_dispatcher<vertex_t, edge_t, weight_t, store_transposed, true>(
        graph_type, f, args...);
    } break;
    case false: {
      return graph_dispatcher<vertex_t, edge_t, weight_t, store_transposed, false>(
        graph_type, f, args...);
    }
  }
}

// transpose bool dispatcher:
// resolves bool `store_transpose`
// and using template arguments vertex_t, edge_t, weight_t
// cascades into next level
// multi_gpu_dispatcher()
//
template <typename vertex_t, typename edge_t, typename weight_t, typename FType, typename... Fargs>
constexpr decltype(auto) transp_dispatcher(
  bool store_transposed, bool multi_gpu, GTypes graph_type, FType f, Fargs&&... args)
{
  switch (store_transposed) {
    case true: {
      return multi_gpu_dispatcher<vertex_t, edge_t, weight_t, true>(
        multi_gpu, graph_type, f, args...);
    } break;
    case false: {
      return multi_gpu_dispatcher<vertex_t, edge_t, weight_t, false>(
        multi_gpu, graph_type, f, args...);
    }
  }
}

// weight type dispatcher:
// resolves weigth_t from weight_type enum
// and using template arguments vertex_t, edge_t
// cascades into next level
// transp_dispatcher()
//
template <typename vertex_t, typename edge_t, typename FType, typename... Fargs>
constexpr decltype(auto) weight_dispatcher(DTypes weight_type,
                                           bool store_transposed,
                                           bool multi_gpu,
                                           GTypes graph_type,
                                           FType f,
                                           Fargs&&... args)
{
  switch (weight_type) {
    case DTypes::INT32: {
      using weight_t = typename DMapType<DTypes::INT32>::type;
      return transp_dispatcher<vertex_t, edge_t, weight_t>(
        store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    case DTypes::INT64: {
      using weight_t = typename DMapType<DTypes::INT64>::type;
      return transp_dispatcher<vertex_t, edge_t, weight_t>(
        store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    case DTypes::FLOAT32: {
      using weight_t = typename DMapType<DTypes::FLOAT32>::type;
      return transp_dispatcher<vertex_t, edge_t, weight_t>(
        store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    case DTypes::FLOAT64: {
      using weight_t = typename DMapType<DTypes::FLOAT64>::type;
      return transp_dispatcher<vertex_t, edge_t, weight_t>(
        store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    default: {
      std::stringstream ss;
      ss << "ERROR: Unknown type enum:" << static_cast<int>(weight_type);
      throw std::runtime_error(ss.str());
    }
  }
}

// edge type dispatcher:
// resolves edge_t from edge_type enum
// and using template argument vertex_t
// cascades into the next level
// weight_dispatcher();
//
template <typename vertex_t, typename FType, typename... Fargs>
constexpr decltype(auto) edge_dispatcher(DTypes edge_type,
                                         DTypes weight_type,
                                         bool store_transposed,
                                         bool multi_gpu,
                                         GTypes graph_type,
                                         FType f,
                                         Fargs&&... args)
{
  switch (edge_type) {
    case DTypes::INT32: {
      using edge_t = typename DMapType<DTypes::INT32>::type;
      return weight_dispatcher<vertex_t, edge_t>(
        weight_type, store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    case DTypes::INT64: {
      using edge_t = typename DMapType<DTypes::INT64>::type;
      return weight_dispatcher<vertex_t, edge_t>(
        weight_type, store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    case DTypes::FLOAT32: {
      using edge_t = typename DMapType<DTypes::FLOAT32>::type;
      return weight_dispatcher<vertex_t, edge_t>(
        weight_type, store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    case DTypes::FLOAT64: {
      using edge_t = typename DMapType<DTypes::FLOAT64>::type;
      return weight_dispatcher<vertex_t, edge_t>(
        weight_type, store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    default: {
      std::stringstream ss;
      ss << "ERROR: Unknown type enum:" << static_cast<int>(edge_type);
      throw std::runtime_error(ss.str());
    }
  }
}

// vertex type dispatcher:
// entry point,
// resolves vertex_t from vertex_type enum
// and  cascades into the next level
// edge_dispatcher();
//
template <typename FType, typename... Fargs>
constexpr decltype(auto) vertex_dispatcher(DTypes vertex_type,
                                           DTypes edge_type,
                                           DTypes weight_type,
                                           bool store_transposed,
                                           bool multi_gpu,
                                           GTypes graph_type,
                                           FType f,
                                           Fargs&&... args)
{
  switch (vertex_type) {
    case DTypes::INT32: {
      using vertex_t = typename DMapType<DTypes::INT32>::type;
      return edge_dispatcher<vertex_t>(
        edge_type, weight_type, store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    case DTypes::INT64: {
      using vertex_t = typename DMapType<DTypes::INT64>::type;
      return edge_dispatcher<vertex_t>(
        edge_type, weight_type, store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    case DTypes::FLOAT32: {
      using vertex_t = typename DMapType<DTypes::FLOAT32>::type;
      return edge_dispatcher<vertex_t>(
        edge_type, weight_type, store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    case DTypes::FLOAT64: {
      using vertex_t = typename DMapType<DTypes::FLOAT64>::type;
      return edge_dispatcher<vertex_t>(
        edge_type, weight_type, store_transposed, multi_gpu, graph_type, f, args...);
    } break;
    default: {
      std::stringstream ss;
      ss << "ERROR: Unknown type enum:" << static_cast<int>(vertex_type);
      throw std::runtime_error(ss.str());
    }
  }
}

}  // namespace experimental
}  // namespace cugraph
