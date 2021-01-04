#include <experimental/graph.hpp>
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
  // unless algorithms only call virtual graph methods
  // under the hood, the algos require this conversion:
  //
  graph_t<vertex_t, edge_t, weight_t, st, mg> const* p_g =
    static_cast<graph_t<vertex_t, edge_t, weight_t, st, mg> const*>(&graph);

  auto const& arg_vec = ep_.get_args();

  // TODO:
  // unpack bfs() args:
  //
  // vertex_t* p_d_clust = static_cast<vertex_t*>(arg_vec[0]);
  // size_t max_lvl      = *static_cast<size_t*>(arg_vec[1]);
  // weight_t resolution = *static_cast<weight_t*>(arg_vec[2]);

  // TODO:
  // call algorithm
  //
  /// auto ret = algorithms::bfs(*p_g, p_d_clust, max_lvl, resolution);
  ///
  /// result_ = return_t{ret};

  /// std::cout << "cython-friendly unpacked args: " << p_d_clust << ", " << max_lvl << ", "
  ///<< resolution << '\n';

  /// std::cout << "...bfs graph_t visitor\n";
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

}  // namespace experimental
}  // namespace cugraph
