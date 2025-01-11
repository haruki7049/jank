#pragma once

#include <jank/runtime/object.hpp>
#include <jank/runtime/detail/native_persistent_list.hpp>
#include <jank/runtime/obj/nil.hpp>
#include <jank/runtime/obj/number.hpp>
#include <jank/runtime/obj/ratio.hpp>
#include <jank/runtime/obj/persistent_list.hpp>
#include <jank/runtime/obj/persistent_string.hpp>
#include <jank/runtime/obj/character.hpp>

namespace jank::runtime
{
  /* TODO: Constexpr these. */
  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(std::nullptr_t const &)
  {
    return runtime::obj::nil::nil_const();
  }

  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(native_bool const b)
  {
    return b ? runtime::obj::boolean::true_const() : runtime::obj::boolean::false_const();
  }

  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(int const i)
  {
    return make_box<runtime::obj::integer>(static_cast<native_integer>(i));
  }

  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(native_integer const i)
  {
    return make_box<runtime::obj::integer>(i);
  }

  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(char const i)
  {
    return make_box<runtime::obj::character>(i);
  }

  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(size_t const i)
  {
    return make_box<runtime::obj::integer>(static_cast<native_integer>(i));
  }

  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(native_real const r)
  {
    return make_box<runtime::obj::real>(r);
  }

  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(obj::ratio_data const &r)
  {
    return make_box<runtime::obj::ratio>(r);
  }

  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(native_persistent_string_view const &s)
  {
    return make_box<runtime::obj::persistent_string>(s);
  }

  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline runtime::obj::persistent_string_ptr make_box(char const * const s)
  {
    assert(s);
    return make_box<runtime::obj::persistent_string>(s);
  }

  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(runtime::detail::native_persistent_list const &l)
  {
    return make_box<runtime::obj::persistent_list>(l);
  }

  template <typename T>
  requires std::is_floating_point_v<T>
  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(T const d)
  {
    return make_box<runtime::obj::real>(d);
  }

  template <typename T>
  requires std::is_integral_v<T>
  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(T const d)
  {
    return make_box<runtime::obj::integer>(d);
  }

  template <typename T>
  requires runtime::behavior::object_like<T>
  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(T * const d)
  {
    return d;
  }

  template <typename T>
  requires runtime::behavior::object_like<T>
  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(T const * const d)
  {
    return d;
  }

  template <typename T>
  requires runtime::behavior::object_like<T>
  [[gnu::always_inline, gnu::flatten, gnu::hot]]
  inline auto make_box(native_box<T> const &d)
  {
    return d;
  }
}
