#pragma once

#include <jank/runtime/obj/number.hpp>
#include <jank/runtime/obj/nil.hpp>

namespace jank::runtime
{
  native_bool truthy(object const *o);
  native_bool truthy(object_ptr o);
  native_bool truthy(obj::nil_ptr);
  native_bool truthy(obj::boolean_ptr const o);
  native_bool truthy(native_bool const o);

  template <typename T>
  requires runtime::behavior::object_like<T>
  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto truthy(T const * const d)
  {
    if constexpr(std::same_as<T, obj::nil>)
    {
      return false;
    }
    else if constexpr(std::same_as<T, obj::boolean>)
    {
      return d->data;
    }
    else
    {
      return true;
    }
  }

  template <typename T>
  requires runtime::behavior::object_like<T>
  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto truthy(native_box<T> const &d)
  {
    return truthy(d.data);
  }
}
