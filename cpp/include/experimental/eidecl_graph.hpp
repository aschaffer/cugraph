#pragma once

namespace cugraph {
namespace experimental {
extern template class graph_t<int32_t, int32_t, float, true, true, void>;
extern template class graph_t<int32_t, int32_t, float, true, false, void>;
extern template class graph_t<int32_t, int32_t, float, false, true, void>;
extern template class graph_t<int32_t, int32_t, float, false, false, void>;
extern template class graph_t<int32_t, int32_t, double, true, true, void>;
extern template class graph_t<int32_t, int32_t, double, true, false, void>;
extern template class graph_t<int32_t, int32_t, double, false, true, void>;
extern template class graph_t<int32_t, int32_t, double, false, false, void>;
extern template class graph_t<int32_t, int64_t, float, true, true, void>;
extern template class graph_t<int32_t, int64_t, float, true, false, void>;
extern template class graph_t<int32_t, int64_t, float, false, true, void>;
extern template class graph_t<int32_t, int64_t, float, false, false, void>;
extern template class graph_t<int32_t, int64_t, double, true, true, void>;
extern template class graph_t<int32_t, int64_t, double, true, false, void>;
extern template class graph_t<int32_t, int64_t, double, false, true, void>;
extern template class graph_t<int32_t, int64_t, double, false, false, void>;
extern template class graph_t<int64_t, int32_t, float, true, true, void>;
extern template class graph_t<int64_t, int32_t, float, true, false, void>;
extern template class graph_t<int64_t, int32_t, float, false, true, void>;
extern template class graph_t<int64_t, int32_t, float, false, false, void>;
extern template class graph_t<int64_t, int32_t, double, true, true, void>;
extern template class graph_t<int64_t, int32_t, double, true, false, void>;
extern template class graph_t<int64_t, int32_t, double, false, true, void>;
extern template class graph_t<int64_t, int32_t, double, false, false, void>;
extern template class graph_t<int64_t, int64_t, float, true, true, void>;
extern template class graph_t<int64_t, int64_t, float, true, false, void>;
extern template class graph_t<int64_t, int64_t, float, false, true, void>;
extern template class graph_t<int64_t, int64_t, float, false, false, void>;
extern template class graph_t<int64_t, int64_t, double, true, true, void>;
extern template class graph_t<int64_t, int64_t, double, true, false, void>;
extern template class graph_t<int64_t, int64_t, double, false, true, void>;
extern template class graph_t<int64_t, int64_t, double, false, false, void>;
extern template class graph_view_t<int32_t, int32_t, float, true, true, void>;
extern template class graph_view_t<int32_t, int32_t, float, true, false, void>;
extern template class graph_view_t<int32_t, int32_t, float, false, true, void>;
extern template class graph_view_t<int32_t, int32_t, float, false, false, void>;
extern template class graph_view_t<int32_t, int32_t, double, true, true, void>;
extern template class graph_view_t<int32_t, int32_t, double, true, false, void>;
extern template class graph_view_t<int32_t, int32_t, double, false, true, void>;
extern template class graph_view_t<int32_t, int32_t, double, false, false, void>;
extern template class graph_view_t<int32_t, int64_t, float, true, true, void>;
extern template class graph_view_t<int32_t, int64_t, float, true, false, void>;
extern template class graph_view_t<int32_t, int64_t, float, false, true, void>;
extern template class graph_view_t<int32_t, int64_t, float, false, false, void>;
extern template class graph_view_t<int32_t, int64_t, double, true, true, void>;
extern template class graph_view_t<int32_t, int64_t, double, true, false, void>;
extern template class graph_view_t<int32_t, int64_t, double, false, true, void>;
extern template class graph_view_t<int32_t, int64_t, double, false, false, void>;
extern template class graph_view_t<int64_t, int32_t, float, true, true, void>;
extern template class graph_view_t<int64_t, int32_t, float, true, false, void>;
extern template class graph_view_t<int64_t, int32_t, float, false, true, void>;
extern template class graph_view_t<int64_t, int32_t, float, false, false, void>;
extern template class graph_view_t<int64_t, int32_t, double, true, true, void>;
extern template class graph_view_t<int64_t, int32_t, double, true, false, void>;
extern template class graph_view_t<int64_t, int32_t, double, false, true, void>;
extern template class graph_view_t<int64_t, int32_t, double, false, false, void>;
extern template class graph_view_t<int64_t, int64_t, float, true, true, void>;
extern template class graph_view_t<int64_t, int64_t, float, true, false, void>;
extern template class graph_view_t<int64_t, int64_t, float, false, true, void>;
extern template class graph_view_t<int64_t, int64_t, float, false, false, void>;
extern template class graph_view_t<int64_t, int64_t, double, true, true, void>;
extern template class graph_view_t<int64_t, int64_t, double, true, false, void>;
extern template class graph_view_t<int64_t, int64_t, double, false, true, void>;
extern template class graph_view_t<int64_t, int64_t, double, false, false, void>;
}  // namespace experimental
}  // namespace cugraph
