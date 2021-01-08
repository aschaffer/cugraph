#pragma once
#include "erased_pack.hpp"
#include "graph_envelope.hpp"
#include "ret_terased.hpp"

namespace cugraph {
namespace experimental {

// macro option: MAKE_VISITOR(bfs)

// primary empty template:
//
template <typename vertex_t,
          typename edge_t,
          typename weight_t,
          bool st,
          bool mg,
          typename Enable = void>
struct bfs_visitor;

// dummy out non-candidate instantiation paths:
//
template <typename vertex_t, typename edge_t, typename weight_t, bool st, bool mg>
struct bfs_visitor<vertex_t,
                   edge_t,
                   weight_t,
                   st,
                   mg,
                   std::enable_if_t<!is_candidate<vertex_t, edge_t, weight_t>::value>> : visitor_t {
  void visit_graph(graph_envelope_t::base_graph_t const&) override
  {
    // purposely empty
  }
  return_t const& get_result(void) const override
  {
    static return_t r{};
    return r;
  }
};

template <typename vertex_t, typename edge_t, typename weight_t, bool st, bool mg>
struct bfs_visitor<vertex_t,
                   edge_t,
                   weight_t,
                   st,
                   mg,
                   std::enable_if_t<is_candidate<vertex_t, edge_t, weight_t>::value>> : visitor_t {
  bfs_visitor(erased_pack_t& ep) : ep_(ep) {}

  void visit_graph(graph_envelope_t::base_graph_t const&) override;

  return_t const& get_result(void) const override { return result_; }

 private:
  erased_pack_t& ep_;
  return_t result_;
};

// wrapper:
//
return_t bfs_wrapper(graph_envelope_t const& g, erased_pack_t& ep);
}  // namespace experimental
}  // namespace cugraph
