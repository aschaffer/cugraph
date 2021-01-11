#pragma once

#include <cstdint>

namespace cugraph {
namespace experimental {

enum class DTypes { INT32 = 0, INT64, FLOAT32, FLOAT64, NTYPES };

template <DTypes>
struct DMapType;

template <>
struct DMapType<DTypes::INT32> {
  using type = int32_t;
};

template <>
struct DMapType<DTypes::INT64> {
  using type = int64_t;
};

template <>
struct DMapType<DTypes::FLOAT32> {
  using type = float;
};

template <>
struct DMapType<DTypes::FLOAT64> {
  using type = double;
};

template <typename T>
struct reverse_dmap_t;

template <>
struct reverse_dmap_t<int32_t> {
  static constexpr DTypes type_id = DTypes::INT32;
};

template <>
struct reverse_dmap_t<int64_t> {
  static constexpr DTypes type_id = DTypes::INT64;
};

template <>
struct reverse_dmap_t<float> {
  static constexpr DTypes type_id = DTypes::FLOAT32;
};

template <>
struct reverse_dmap_t<double> {
  static constexpr DTypes type_id = DTypes::FLOAT64;
};

}  // namespace experimental
}  // namespace cugraph
