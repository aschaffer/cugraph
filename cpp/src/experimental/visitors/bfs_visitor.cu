#include <experimental/graph.hpp>
#include <experimental/visitors/bfs_visitor.cuh>
#include <experimental/visitors/bfs_visitor.hpp>

namespace cugraph {
namespace experimental {

//
// wrapper code:
//
template <typename vertex_t, typename edge_t, typename weight_t, bool st, bool mg>
void bfs_visitor<vertex_t,
                 edge_t,
                 weight_t,
                 st,
                 mg,
                 std::enable_if_t<is_candidate<vertex_t, edge_t, weight_t>::value>>::
  visit_graph(graph_envelope_t::base_graph_t const& graph)
{
  // TODO: check compile-time branch at runtime trick...
  //

  // unless algorithms only call virtual graph methods
  // under the hood, the algos require this conversion:
  //
  graph_t<vertex_t, edge_t, weight_t, st, mg> const* p_g =
    static_cast<graph_t<vertex_t, edge_t, weight_t, st, mg> const*>(&graph);

  // Note: this must be called only on:
  // graph_view_t<vertex_t, edge_t, weight_t, false, mg>
  // which requires the "no-op" overload of bfs_low_level()
  // in `bfs_visitor.cuh`;
  //
  auto gview = p_g->view();

  auto const& v_args = ep_.get_args();

  // unpack bfs() args:
  //
  assert(v_args.size() == 7);

  // cnstr. args unpacking:
  //
  raft::handle_t const& handle = *static_cast<raft::handle_t const*>(v_args[0]);

  vertex_t* p_d_dist = static_cast<vertex_t*>(v_args[1]);

  vertex_t* p_d_predec = static_cast<vertex_t*>(v_args[2]);

  vertex_t src_v = *static_cast<vertex_t*>(v_args[3]);

  bool dir_opt = *static_cast<bool*>(v_args[4]);

  auto depth_l = *static_cast<vertex_t*>(v_args[5]);

  bool check = *static_cast<bool*>(v_args[6]);

  // call algorithm
  // (no result; void)
  //
  algorithms::bfs_low_level(handle, gview, p_d_dist, p_d_predec, src_v, dir_opt, depth_l, check);
}

// EIDir's:
//
template class bfs_visitor<int, int, float, true, true>;
template class bfs_visitor<int, int, double, true, true>;

template class bfs_visitor<int, int, float, true, false>;
template class bfs_visitor<int, int, double, true, false>;

template class bfs_visitor<int, int, float, false, true>;
template class bfs_visitor<int, int, double, false, true>;

template class bfs_visitor<int, int, float, false, false>;
template class bfs_visitor<int, int, double, false, false>;

//------

template class bfs_visitor<int, long, float, true, true>;
template class bfs_visitor<int, long, double, true, true>;

template class bfs_visitor<int, long, float, true, false>;
template class bfs_visitor<int, long, double, true, false>;

template class bfs_visitor<int, long, float, false, true>;
template class bfs_visitor<int, long, double, false, true>;

template class bfs_visitor<int, long, float, false, false>;
template class bfs_visitor<int, long, double, false, false>;

//------

template class bfs_visitor<long, long, float, true, true>;
template class bfs_visitor<long, long, double, true, true>;

template class bfs_visitor<long, long, float, true, false>;
template class bfs_visitor<long, long, double, true, false>;

template class bfs_visitor<long, long, float, false, true>;
template class bfs_visitor<long, long, double, false, true>;

template class bfs_visitor<long, long, float, false, false>;
template class bfs_visitor<long, long, double, false, false>;

// wrapper:
//
return_t bfs_wrapper(graph_envelope_t const& g, erased_pack_t& ep)
{
  auto p_visitor = g.factory()->make_bfs_visitor(ep);

  g.apply(*p_visitor);

  return_t ret{p_visitor->get_result()};

  return ret;  // RVO-ed;
}

}  // namespace experimental
}  // namespace cugraph
