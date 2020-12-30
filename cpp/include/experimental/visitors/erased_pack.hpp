#pragma once

#define _DEBUG_

#ifdef _DEBUG_
#include <iostream>
#endif

#include <initializer_list>
#include <vector>

namespace cugraph {
namespace experimental {

struct erased_pack_t {
  erased_pack_t(void** p_args, size_t n)
    : args_{[](void** p, size_t n) {
        std::vector<void*> v;
        v.insert(v.begin(), p, p + n);
        return v;
      }(p_args, n)}
  {
    // args_.insert(args_.begin(), p_args, p_args + n);
  }

  erased_pack_t(std::initializer_list<void*> args) : args_{args} {}

  std::vector<void*> const& get_args(void) const { return args_; }

  erased_pack_t(erased_pack_t const&) = delete;
  erased_pack_t& operator=(erased_pack_t const&) = delete;

  erased_pack_t(erased_pack_t&& other) : args_(std::move(other.args_)) {}

  erased_pack_t& operator=(erased_pack_t&& other)
  {
    args_ = std::move(other.args_);
    return *this;
  }

#ifdef _DEBUG_
  void print(void) const
  {
    std::cout << "list args addresses:\n";
    for (auto&& elem : args_) std::cout << elem << ", ";
    std::cout << '\n';
  }
#endif

 private:
  std::vector<void*> args_;
};

}  // namespace experimental
}  // namespace cugraph
