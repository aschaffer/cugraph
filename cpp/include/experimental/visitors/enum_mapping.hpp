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

}  // namespace experimental
}  // namespace cugraph
