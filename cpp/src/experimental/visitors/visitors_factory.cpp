#include <experimental/visitors/bfs_visitor.hpp>
#include <experimental/visitors/graph_envelope.hpp>

namespace cugraph {
namespace experimental {

template <typename vertex_t, typename edge_t, typename weight_t, bool st, bool mg>
std::unique_ptr<visitor_t>
dependent_factory_t<vertex_t,
                    edge_t,
                    weight_t,
                    st,
                    mg,
                    std::enable_if_t<is_candidate<vertex_t, edge_t, weight_t>::value>>::
  make_louvain_visitor(erased_pack_t& ep) const
{
  /// return std::unique_ptr<visitor_t>(
  ///  static_cast<visitor_t*>(new louvain_visitor<vertex_t, edge_t, weight_t, st, mg>(ep)));

  return nullptr;  // for now...
}

template <typename vertex_t, typename edge_t, typename weight_t, bool st, bool mg>
std::unique_ptr<visitor_t>
dependent_factory_t<vertex_t,
                    edge_t,
                    weight_t,
                    st,
                    mg,
                    std::enable_if_t<is_candidate<vertex_t, edge_t, weight_t>::value>>::
  make_bfs_visitor(erased_pack_t& ep) const
{
  // return nullptr;  // for now...
  return std::make_unique<bfs_visitor<vertex_t, edge_t, weight_t, st, mg>>(ep);
}

// EIDir's:
//
template class dependent_factory_t<int, int, float, true, true>;
template class dependent_factory_t<int, int, double, true, true>;

template class dependent_factory_t<int, int, float, true, false>;
template class dependent_factory_t<int, int, double, true, false>;

template class dependent_factory_t<int, int, float, false, true>;
template class dependent_factory_t<int, int, double, false, true>;

template class dependent_factory_t<int, int, float, false, false>;
template class dependent_factory_t<int, int, double, false, false>;

//------

template class dependent_factory_t<int, long, float, true, true>;
template class dependent_factory_t<int, long, double, true, true>;

template class dependent_factory_t<int, long, float, true, false>;
template class dependent_factory_t<int, long, double, true, false>;

template class dependent_factory_t<int, long, float, false, true>;
template class dependent_factory_t<int, long, double, false, true>;

template class dependent_factory_t<int, long, float, false, false>;
template class dependent_factory_t<int, long, double, false, false>;

//------

template class dependent_factory_t<long, long, float, true, true>;
template class dependent_factory_t<long, long, double, true, true>;

template class dependent_factory_t<long, long, float, true, false>;
template class dependent_factory_t<long, long, double, true, false>;

template class dependent_factory_t<long, long, float, false, true>;
template class dependent_factory_t<long, long, double, false, true>;

template class dependent_factory_t<long, long, float, false, false>;
template class dependent_factory_t<long, long, double, false, false>;

// Either use EIDir or specialization, can't have both;
// Prefer specialization when EIdir's are not enough
// because of cascaded-dispatcher exhaustive instantiations
// In this case EIDir above are enough;
}  // namespace experimental
}  // namespace cugraph
