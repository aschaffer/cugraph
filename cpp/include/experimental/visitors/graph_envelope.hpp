// Goal: be able to call an algorithm (say. louvain() on a type erased graph created from RTTI:
//{
// auto graph = make_graph(flags...);
// auto res = louvain(graph, params...);
//}
// params will be also type-erased (or same type regardless of graph-type); and will
// be appropriately passed to the Factory and then converted and passed to Visitor constructor

#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "enum_mapping.hpp"
#include "graph_enum.hpp"

namespace cugraph {
namespace experimental {

// visitor base, incomplete:
//
class visitor_t;  // forward...

// envelope class around all
// graph classes:
//
struct graph_envelope_t {
  struct base_graph_t {
    virtual ~base_graph_t() {}

    // virtual void apply(visitor_t& v) = 0;

    virtual void apply(visitor_t& v) const = 0;
  };

  // abstract factory:
  //
  struct visitor_factory_t {
    virtual std::unique_ptr<visitor_t> make_bfs_visitor(
      void* p_vt_arr_dst,
      void* p_vt_arr_prec,
      void* p_wt_arr_sp_count,
      void const* p_vt_scalar_src_v,
      bool dir,
      bool mg_batch) const = 0;  // BFS list of args: must take t-erased list of args for BFS
  };

  using pair_uniques_t =
    std::pair<std::unique_ptr<base_graph_t>, std::unique_ptr<visitor_factory_t>>;

  void apply(visitor_t& v) const
  {
    if (p_impl_fact_.first)
      p_impl_fact_.first->apply(v);
    else
      throw std::runtime_error("ERROR: Implementation not allocated.");
  }

  std::unique_ptr<visitor_factory_t> const& factory(void) const { return p_impl_fact_.second; }

  // place in TU and use manual instantiation (EIDir):
  //
  template <typename... Args>
  graph_envelope_t(DTypes vertex_tid,
                   DTypes edge_tid,
                   DTypes weight_tid,
                   bool store_transpose,
                   bool multi_gpu,
                   GTypes graph_tid,
                   Args&&... args);

 private:
  // need it to hide the parameterization of
  // (graph implementation, factory implementation)
  // by dependent types: vertex_t, edge_t, weight_t
  //
  pair_uniques_t p_impl_fact_;
};

// visitor base:
//
class visitor_t {
 public:
  virtual ~visitor_t(void) {}

  virtual void visit_graph_t(graph_envelope_t::base_graph_t const&) = 0;

  virtual void const* get_result(void) const = 0;
};

// convenience templatized base:
//
template <typename vertex_t, typename edge_t, typename weight_t>
struct dependent_graph_t : graph_envelope_t::base_graph_t {
  using vertex_type = vertex_t;
  using edge_type   = edge_t;
  using weight_type = weight_t;
};

template <typename vertex_t, typename edge_t, typename weight_t>
struct dependent_factory_t : graph_envelope_t::visitor_factory_t {
  using vertex_type = vertex_t;
  using edge_type   = edge_t;
  using weight_type = weight_t;

  std::unique_ptr<visitor_t> make_bfs_visitor(void* p_vt_arr_dst,
                                              void* p_vt_arr_prec,
                                              void* p_wt_arr_sp_count,
                                              void const* p_vt_scalar_src_v,
                                              bool dir,
                                              bool mg_batch) const override
  {
    // no-op...actual work left for specializations inside visitors_factory.cpp
    // this is to make linker happy:
    // because of cascaded-
    // dispatcher exhaustive instantiations
    //
    return nullptr;
  }
};

// EIDecl:
//
// extern template graph_envelope_t::graph_envelope_t<>(
//  DTypes vertex_tid, DTypes edge_tid, DTypes weight_tid, bool, bool, GTypes graph_tid);

// extern template graph_envelope_t::graph_envelope_t<>(DTypes vertex_tid,
//                                                      DTypes edge_tid,
//                                                      DTypes weight_tid,
//                                                      bool,
//                                                      bool,
//                                                      GTypes graph_tid,
//                                                      void*&&,
//                                                      void*&&,
//                                                      void*&&);
}  // namespace experimental
}  // namespace cugraph
