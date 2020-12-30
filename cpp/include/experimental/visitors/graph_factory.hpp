#pragma once

#include <stdexcept>

#include "graph_envelope.hpp"
// prevent clang-format to rearange order of headers
#include "erased_pack.hpp"
//
// not really needed here;
// just to make happy the clang-format policy
// of header inclusion to be order-independent...
//
#include <experimental/graph.hpp>

namespace cugraph {
namespace experimental {

struct graph_factory_base_t {
  virtual ~graph_factory_base_t(void) {}

  virtual std::unique_ptr<graph_envelope_t::base_graph_t> make_graph(erased_pack_t&) const = 0;
};

// straw factory; to be (partiallY) specialized;
// and explicitly instantiated for concrete graphs
//
template <typename graph_type>
struct graph_factory_t : graph_factory_base_t {
  std::unique_ptr<graph_envelope_t::base_graph_t> make_graph(erased_pack_t&) const override
  {
    throw std::runtime_error("Empty factory, not to be called...");
  }
};

// Linker PROBLEM (FIXED):
// dispatcher needs _ALL_ paths instantiated,
// not just the ones explicitly instantiated
// (EIDir) in `graph.cpp`
//
// SOLUTIONS:
//
// (1.) the _factory_ must provide "dummy"
//      instantiations for paths not needed;
//
// or:
//
// (2.) the _dispatcher_ (graph_dispatcher())
//      must "dummy-out" the paths not needed; (Done!)
//
template <typename vertex_t,
          typename edge_t,
          typename weight_t,
          bool store_transposed,
          bool multi_gpu>
struct graph_factory_t<
  graph_t<vertex_t, edge_t, weight_t, store_transposed, multi_gpu, std::enable_if_t<multi_gpu>>>
  : graph_factory_base_t {
  std::unique_ptr<graph_envelope_t::base_graph_t> make_graph(erased_pack_t& ep) const override
  {
    /// std::cout << "Multi-GPU factory.\n";
    std::vector<void*> const& v_args{ep.get_args()};

    assert(v_args.size() == 8);

    // cnstr. args unpacking:
    //
    raft::handle_t const& handle = *static_cast<raft::handle_t const*>(v_args[0]);

    auto const& elist =
      *static_cast<std::vector<edgelist_t<vertex_t, edge_t, weight_t>> const*>(v_args[1]);

    auto const& partition = *static_cast<partition_t<vertex_t> const*>(v_args[2]);

    auto nv = *static_cast<vertex_t*>(v_args[3]);

    auto ne = *static_cast<edge_t*>(v_args[4]);

    auto props = *static_cast<graph_properties_t*>(v_args[5]);

    bool sorted = *static_cast<bool*>(v_args[6]);

    bool check = *static_cast<bool*>(v_args[7]);

    // when a `graph_t<>` instantiation path has more than one
    // cnstr. then must dispatch `graph_t<>` cnstr. based on
    // `ep.pack_id()`; not the case here, because the 2 different
    // `grph_t` constructors each belong to a different `graph_t`
    // instantiation;
    //
    // FIXED: linker error because of PROBLEM above...
    // i.e., when there's no `graph_t<int, int, int,...>::graph_t(...)`, etc.
    // because there's no instantiation of `graph_t` with `weight_t = int`, etc.
    //
    return std::make_unique<graph_t<vertex_t, edge_t, weight_t, store_transposed, multi_gpu>>(
      handle, elist, partition, nv, ne, props, sorted, check);
  }
};

template <typename vertex_t,
          typename edge_t,
          typename weight_t,
          bool store_transposed,
          bool multi_gpu>
struct graph_factory_t<
  graph_t<vertex_t, edge_t, weight_t, store_transposed, multi_gpu, std::enable_if_t<!multi_gpu>>>
  : graph_factory_base_t {
  std::unique_ptr<graph_envelope_t::base_graph_t> make_graph(erased_pack_t& ep) const override
  {
    /// std::cout << "Single-GPU factory.\n";
    std::vector<void*> const& v_args{ep.get_args()};

    assert(v_args.size() == 6);

    raft::handle_t const& handle = *static_cast<raft::handle_t const*>(v_args[0]);

    auto const& elist = *static_cast<edgelist_t<vertex_t, edge_t, weight_t> const*>(v_args[1]);

    auto nv = *static_cast<vertex_t*>(v_args[2]);

    auto props = *static_cast<graph_properties_t*>(v_args[3]);

    bool sorted = *static_cast<bool*>(v_args[4]);

    bool check = *static_cast<bool*>(v_args[5]);

    return std::make_unique<graph_t<vertex_t, edge_t, weight_t, store_transposed, multi_gpu>>(
      handle, elist, nv, props, sorted, check);

    // return nullptr;  // for now...
    // Might need TODO: dispatch graph_t<> cnstr. based on ep.pack_id()
  }
};

}  // namespace experimental
}  // namespace cugraph
