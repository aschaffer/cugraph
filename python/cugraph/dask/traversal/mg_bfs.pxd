#
# Copyright (c) 2020, NVIDIA CORPORATION.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from cugraph.structure.graph_primtypes cimport *
from libcpp cimport bool


cdef extern from "utilities/cython.hpp" namespace "cugraph::cython":

    cdef void call_bfs[vertex_t, weight_t](
        const handle_t &handle,
        const graph_container_t &g,
        vertex_t *identifiers,
        vertex_t *distances,
        vertex_t *predecessors,
        double *sp_counters,
        const vertex_t start_vertex,
        bool directed) except +

# TODO:
#
# `cdef extern erased_pack_t`:
#

cdef extern from "experimental/visitors/erased_pack.hpp" namespace "cugraph::experimental":

    cdef cppclass erased_pack_t:
        erased_pack_t(void** p_args, size_t n)

# enums:
#  DTypes:
#
cdef extern from "experimental/visitors/enum_mapping.hpp" namespace "cugraph::experimental":

    ctypedef enum DTypes:
        INT32 "cugraph::experimental::INT32"
        INT64 "cugraph::experimental::INT64"
        FLOAT32 "cugraph::experimental::FLOAT32"
        FLOAT64 "cugraph::experimental::FLOAT64"

#  GTypes:
#
cdef extern from "experimental/visitors/graph_enum.hpp" namespace "cugraph::experimental":

    ctypedef enum GTypes:
        GRAPH_T "cugraph::experimental::GRAPH_T"
        GRAPH_VIEW_T "cugraph::experimental::GRAPH_VIEW_T"

# `cdef extern graph_envelope_t`:
#
cdef extern from "experimental/visitors/graph_envelope.hpp" namespace "cugraph::experimental":

    cdef cppclass graph_envelope_t:
        graph_envelope_t(DTypes vertex_tid, DTypes edge_tid, DTypes weight_tid, bool, bool, GTypes graph_tid, erased_pack_t&)

# `cdef extern return_t`
cdef extern from "experimental/visitors/ret_terased.hpp" namespace "cugraph::experimental":

    cdef cppclass return_t:
        return_t()
        return_t(const return_t&)


# `from libcpp.vector cimport vector`
#
# initialize `erased_pack_t` via `vector`
# to be filled via push_back(), which
# should work after the `cimport` above;
# all onjects without trivial constructors
# need to be allocated with `new`
#
cdef extern from "experimental/visitors/bfs_visitor.hpp" namespace "cugraph::experimental":

    cdef return_t bfs_wrapper(const graph_envelope_t &g, erased_pack_t& ep) except +
