#include <experimental/visitors/cascaded_dispatch.hpp>
#include <experimental/visitors/graph_envelope.hpp>

namespace cugraph {
namespace experimental {

// call cascaded dispatcher with factory and erased_pack_t
//
graph_envelope_t::graph_envelope_t(DTypes vertex_tid,
                                   DTypes edge_tid,
                                   DTypes weight_tid,
                                   bool st,
                                   bool mg,
                                   GTypes graph_tid,
                                   erased_pack_t& ep)
  : p_impl_fact_(vertex_dispatcher(vertex_tid, edge_tid, weight_tid, st, mg, graph_tid, ep))
{
}

template class graph_factory_t<graph_t<int, int, float, true, true>>;
template class graph_factory_t<graph_t<int, int, double, true, true>>;

template class graph_factory_t<graph_t<int, int, float, true, false>>;
template class graph_factory_t<graph_t<int, int, double, true, false>>;

template class graph_factory_t<graph_t<int, int, float, false, true>>;
template class graph_factory_t<graph_t<int, int, double, false, true>>;

template class graph_factory_t<graph_t<int, int, float, false, false>>;
template class graph_factory_t<graph_t<int, int, double, false, false>>;

}  // namespace experimental
}  // namespace cugraph
