#include <experimental/visitors/graph_envelope.hpp>
//#include <experimental/visitors/bfs_visitor.hpp> // <- TODO:

namespace cugraph {
namespace experimental {

// Cannot use EIDir, must use specialization:
// EIdir are not enough because of cascaded-
// dispatcher exhaustive instantiations
//
template <>
std::unique_ptr<visitor_t> dependent_factory_t<int, int, float>::make_bfs_visitor(
  void* p_vt_arr_dst,
  void* p_vt_arr_prec,
  void* p_wt_arr_sp_count,
  void const* p_vt_scalar_src_v,
  bool dir,
  bool mg_batch) const
{
  using vertex_t = int;
  using edge_t   = int;
  using weight_t = float;

  return nullptr;  // for now...
}

template <>
std::unique_ptr<visitor_t> dependent_factory_t<int, int, double>::make_bfs_visitor(
  void* p_vt_arr_dst,
  void* p_vt_arr_prec,
  void* p_wt_arr_sp_count,
  void const* p_vt_scalar_src_v,
  bool dir,
  bool mg_batch) const
{
  using vertex_t = int;
  using edge_t   = int;
  using weight_t = double;

  return nullptr;  // for now...
}

// template class dependent_factory_t<int, int, float>;
// template class dependent_factory_t<int, int, double>;

}  // namespace experimental
}  // namespace cugraph
