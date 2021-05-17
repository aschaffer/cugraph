/*
 * Copyright (c) 2021, NVIDIA CORPORATION.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Andrei Schaffer, aschaffer@nvidia.com
//

#include <cugraph/serialization/serializer.hpp>

#include <utilities/graph_utils.cuh>

#include <raft/device_atomics.cuh>

#include <rmm/thrust_rmm_allocator.h>

#include <rmm/exec_policy.hpp>

#include <thrust/copy.h>

namespace cugraph {
namespace serializer {
template <typename value_t>
void serializer_t::serialize(value_t const* p_d_src, size_t size)
{
  auto byte_buff_sz       = size * sizeof(value_t);
  auto it_end             = begin_ + byte_buff_sz;
  byte_t const* byte_buff = reinterpret_cast<byte_t const*>(p_d_src);

  thrust::copy_n(rmm::exec_policy(handle_.get_stream_view()), byte_buff, byte_buff_sz, begin_);

  begin_ = it_end;
}

template <typename value_t>
rmm::device_uvector<value_t> serializer_t::unserialize(size_t size)
{
  auto byte_buff_sz = size * sizeof(value_t);
  rmm::device_uvector<value_t> d_dest(size, handle_.get_stream());
  byte_t* byte_buff = reinterpret_cast<byte_t*>(d_dest.data());

  thrust::copy_n(rmm::exec_policy(handle_.get_stream_view()), cbegin_, byte_buff_sz, byte_buff);

  cbegin_ += byte_buff_sz;
  return d_dest;
}

template <typename graph_t>
serializer_t::graph_meta_t<graph_t> serializer_t::serialize(graph_t const& graph)
{
  using vertex_t = typename graph_t::vertex_type;
  using edge_t   = typename graph_t::edge_type;
  using weight_t = typename graph_t::weight_type;

  if constexpr (!graph_t::is_multi_gpu) {
    size_t num_vertices = graph.get_number_of_vertices();
    size_t num_edges    = graph.get_number_of_edges();
    auto g_props        = graph.get_graph_properties();
    auto&& gview        = graph.view();
    auto seg_offsets    = gview.get_local_adj_matrix_partition_segment_offsets(0);
    graph_meta_t<graph_t> gvmeta{num_vertices, num_edges, g_props, seg_offsets};

    edge_t const* offsets   = gview.offsets();
    vertex_t const* indices = gview.indices();
    weight_t const* weights = gview.weights();

    serialize(offsets, num_vertices + 1);
    serialize(indices, num_edges);
    serialize(weights, num_edges);

    return gvmeta;

  } else {
    CUGRAPH_FAIL("Unsupported graph type for serialization.");

    return graph_meta_t<graph_t>{};
  }
}

template <typename graph_t>
graph_t serializer_t::unserialize(serializer_t::graph_meta_t<graph_t> const& gvmeta)
{
  using vertex_t = typename graph_t::vertex_type;
  using edge_t   = typename graph_t::edge_type;
  using weight_t = typename graph_t::weight_type;

  if constexpr (!graph_t::is_multi_gpu) {
    vertex_t num_vertices = gvmeta.num_vertices_;
    edge_t num_edges      = gvmeta.num_edges_;
    auto g_props          = gvmeta.properties_;
    auto seg_offsets      = gvmeta.segment_offsets_;

    auto d_offsets = unserialize<edge_t>(num_vertices + 1);
    auto d_indices = unserialize<vertex_t>(num_edges);
    auto d_weights = unserialize<weight_t>(num_edges);

    return graph_t(handle_,
                   num_vertices,
                   num_edges,
                   g_props,
                   std::move(d_offsets),
                   std::move(d_indices),
                   std::move(d_weights),
                   std::move(seg_offsets));  // RVO-ed

  } else {
    CUGRAPH_FAIL("Unsupported graph type for unserialization.");

    return graph_t{handle_};
  }
}

// Manual template instantiations (EIDir's):
//
template void serializer_t::serialize(int32_t const* p_d_src, size_t size);
template void serializer_t::serialize(int64_t const* p_d_src, size_t size);
template void serializer_t::serialize(float const* p_d_src, size_t size);
template void serializer_t::serialize(double const* p_d_src, size_t size);

template rmm::device_uvector<int32_t> serializer_t::unserialize(size_t size);
template rmm::device_uvector<int64_t> serializer_t::unserialize(size_t size);
template rmm::device_uvector<float> serializer_t::unserialize(size_t size);
template rmm::device_uvector<double> serializer_t::unserialize(size_t size);

// serialize graph:
//
template serializer_t::graph_meta_t<graph_t<int32_t, int32_t, float, false, false>>
serializer_t::serialize(graph_t<int32_t, int32_t, float, false, false> const& graph);

template serializer_t::graph_meta_t<graph_t<int32_t, int64_t, float, false, false>>
serializer_t::serialize(graph_t<int32_t, int64_t, float, false, false> const& graph);

template serializer_t::graph_meta_t<graph_t<int64_t, int64_t, float, false, false>>
serializer_t::serialize(graph_t<int64_t, int64_t, float, false, false> const& graph);

template serializer_t::graph_meta_t<graph_t<int32_t, int32_t, double, false, false>>
serializer_t::serialize(graph_t<int32_t, int32_t, double, false, false> const& graph);

template serializer_t::graph_meta_t<graph_t<int32_t, int64_t, double, false, false>>
serializer_t::serialize(graph_t<int32_t, int64_t, double, false, false> const& graph);

template serializer_t::graph_meta_t<graph_t<int64_t, int64_t, double, false, false>>
serializer_t::serialize(graph_t<int64_t, int64_t, double, false, false> const& graph);

// unserialize graph:
//
template graph_t<int32_t, int32_t, float, false, false> serializer_t::unserialize(
  serializer_t::graph_meta_t<graph_t<int32_t, int32_t, float, false, false>> const& gvmeta);

template graph_t<int32_t, int64_t, float, false, false> serializer_t::unserialize(
  serializer_t::graph_meta_t<graph_t<int32_t, int64_t, float, false, false>> const& gvmeta);

template graph_t<int64_t, int64_t, float, false, false> serializer_t::unserialize(
  serializer_t::graph_meta_t<graph_t<int64_t, int64_t, float, false, false>> const& gvmeta);

template graph_t<int32_t, int32_t, double, false, false> serializer_t::unserialize(
  serializer_t::graph_meta_t<graph_t<int32_t, int32_t, double, false, false>> const& gvmeta);

template graph_t<int32_t, int64_t, double, false, false> serializer_t::unserialize(
  serializer_t::graph_meta_t<graph_t<int32_t, int64_t, double, false, false>> const& gvmeta);

template graph_t<int64_t, int64_t, double, false, false> serializer_t::unserialize(
  serializer_t::graph_meta_t<graph_t<int64_t, int64_t, double, false, false>> const& gvmeta);

}  // namespace serializer
}  // namespace cugraph