#include <experimental/visitors/cascaded_dispatch.hpp>
#include <experimental/visitors/graph_envelope.hpp>
#include <iostream>

namespace cugraph {
namespace experimental {

// pair(graph, visitor factory) factory functor
// to be called by cascaded dispatch:
//
struct factory_t {
  using pair_uniques_t = graph_envelope_t::pair_uniques_t;

  template <typename graph_type_t, typename... Args>
  pair_uniques_t operator()(Args&&... args)
  {
    using vertex_t = typename graph_type_t::vertex_type;
    using edge_t   = typename graph_type_t::edge_type;
    using weight_t = typename graph_type_t::weight_type;

    // note: operator() cannot be aware that `graph_t`
    // is a template class;
    // the whole point is to hide that dependency;
    // consequently, the dispatcher needs
    // to be able to instantiate all `graph_t` instances
    //
    pair_uniques_t p_uniques = std::make_pair(
      std::unique_ptr<graph_envelope_t::base_graph_t>(static_cast<graph_envelope_t::base_graph_t*>(
        new graph_type_t(std::forward<Args>(args)...))),
      std::unique_ptr<graph_envelope_t::visitor_factory_t>(
        static_cast<graph_envelope_t::visitor_factory_t*>(
          new dependent_factory_t<vertex_t, edge_t, weight_t>())));

    return p_uniques;
  }
};

// call cascaded dispatcher with factory_t functor and ...args
//
template <typename... Args>
graph_envelope_t::graph_envelope_t(DTypes vertex_tid,
                                   DTypes edge_tid,
                                   DTypes weight_tid,
                                   bool store_transpose,
                                   bool multi_gpu,
                                   GTypes graph_tid,
                                   Args&&... args)
  : p_impl_fact_(vertex_dispatcher(vertex_tid,
                                   edge_tid,
                                   weight_tid,
                                   store_transpose,
                                   multi_gpu,
                                   graph_tid,
                                   factory_t{},
                                   args...))
{
}

//(Manual) Explicit instantiations (EIDir):
//
// Note:
// Constraints
// {
//  1.  all graph types must supply constructors taking the list of parameters
//      that follow the `graph_tid` parameter
//      (with / without qulaifiers, e.g., `const` `&`, `volatile`);
//      (i.e., as the case is below:
//      `graph_t(void)` and:
//      `graph_t(int, double)` or:
//      `graph_t(int const&, double&)` or, etc.
//
//  2.  all instantiations below (except first one)
//      must be in sync with individual graph types instantiations;
// }
//
// template graph_envelope_t::graph_envelope_t<>(
//  DTypes vertex_tid, DTypes edge_tid, DTypes weight_tid, bool, bool, GTypes graph_tid);

// template graph_envelope_t::graph_envelope_t<>(DTypes vertex_tid,
//                                               DTypes edge_tid,
//                                               DTypes weight_tid,
//                                               bool,
//                                               bool,
//                                               GTypes graph_tid,
//                                               void*&&,
//                                               void*&&,
//                                               void*&&);

}  // namespace experimental
}  // namespace cugraph
