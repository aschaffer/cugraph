#include <memory>

struct return_t
{
  struct base_return_t
  {
    virtual ~base_return_t(void){}

    virtual void copy(void const*) = 0;
  };

  template<typename T>
  struct generic_return_t : base_return_t
  {
    generic_return_t(T const& t)
      : return_(t)
    {
    }

    void copy(void const* p_erased) override
    {
      T const* p_T = static_cast<T const*>(p_erased);
      return_ = *p_T;
    }

    T const& get(void) const
    {
      return return_;
    }
  private:
    T return_;
  };

  template<typename T>
  return_t(T const& t)
    : p_impl_(std::unique_ptr<base_return_t>(static_cast<base_return_t*>(new generic_return_t<T>(t))))
  {
  }

  void copy(void const* p_erased)
  {
    if( p_impl_ )
      p_impl_->copy(p_erased);
  }

  template<typename T>
  T get(void) const
  {
    if( p_impl_ )
      {
        generic_return_t<T> const* p =
          static_cast<generic_return_t<T> const*>(p_impl_.get());
        return p->get();
      }
    else
      return T{};
  }
  
private:
  std::unique_ptr<base_return_t> p_impl_;
};
