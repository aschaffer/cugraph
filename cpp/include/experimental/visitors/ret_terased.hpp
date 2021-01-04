#pragma once

#include <memory>
#include <stdexcept>

namespace cugraph {
namespace experimental {

struct return_t {
  struct base_return_t {
    virtual ~base_return_t(void) {}

    virtual void copy(return_t const&)                       = 0;
    virtual std::unique_ptr<base_return_t> clone(void) const = 0;
  };

  template <typename T>
  struct generic_return_t : base_return_t {
    generic_return_t(T const& t) : return_(t) {}

    void copy(return_t const& r) override
    {
      base_return_t const* p_B = static_cast<base_return_t const*>(r.p_impl_.get());
      return_                  = *(dynamic_cast<T const*>(p_B));
    }

    std::unique_ptr<base_return_t> clone(void) const override
    {
      return std::make_unique<generic_return_t<T>>(return_);
    }

    T const& get(void) const { return return_; }

   private:
    T return_;
  };

  return_t(void) = default;

  template <typename T>
  return_t(T const& t) : p_impl_(std::make_unique<generic_return_t<T>>(t))
  {
  }

  return_t(return_t const& r) : p_impl_{r.clone()} {}

  return_t& operator=(return_t const& r)
  {
    p_impl_ = r.clone();
    return *this;
  }

  return_t(return_t&& other) : p_impl_(std::move(other.p_impl_)) {}
  return_t& operator=(return_t&& other)
  {
    p_impl_ = std::move(other.p_impl_);
    return *this;
  }

  std::unique_ptr<base_return_t> clone(void) const
  {
    if (p_impl_)
      return p_impl_->clone();
    else
      return nullptr;
  }

  template <typename T>
  T get(void) const
  {
    if (p_impl_) {
      generic_return_t<T> const* p = static_cast<generic_return_t<T> const*>(p_impl_.get());
      return p->get();
    } else
      throw std::runtime_error("ERROR: nullptr impl.");
  }

 private:
  std::unique_ptr<base_return_t> p_impl_;
};

}  // namespace experimental
}  // namespace cugraph
