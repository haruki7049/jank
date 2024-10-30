#include <jank/runtime/obj/nil.hpp>

namespace jank::runtime
{
  obj::nil_ptr obj::nil::nil_const()
  {
    static obj::nil r{};
    return &r;
  }

  native_bool obj::nil::equal(object const &o) const
  {
    return &o == &base;
  }

  native_persistent_string const &obj::nil::to_string() const
  {
    static native_persistent_string s{ "nil" };
    return s;
  }

  native_persistent_string const &obj::nil::to_code_string() const
  {
    return to_string();
  }

  void obj::nil::to_string(fmt::memory_buffer &buff) const
  {
    fmt::format_to(std::back_inserter(buff), "nil");
  }

  native_hash obj::nil::to_hash() const
  {
    return 0;
  }

  native_integer obj::nil::compare(object const &o) const
  {
    return (o.type == object_type::nil ? 0 : -1);
  }

  native_integer obj::nil::compare(obj::nil const &) const
  {
    return 0;
  }

  object_ptr obj::nil::get(object_ptr const)
  {
    return &base;
  }

  object_ptr obj::nil::get(object_ptr const, object_ptr const fallback)
  {
    return fallback;
  }

  object_ptr obj::nil::get_entry(object_ptr)
  {
    return &base;
  }

  native_bool obj::nil::contains(object_ptr) const
  {
    return false;
  }

  obj::persistent_array_map_ptr obj::nil::assoc(object_ptr const key, object_ptr const val) const
  {
    return obj::persistent_array_map::create_unique(key, val);
  }

  obj::nil_ptr obj::nil::dissoc(object_ptr const) const
  {
    return this;
  }

  obj::nil_ptr obj::nil::seq()
  {
    return nullptr;
  }

  obj::nil_ptr obj::nil::fresh_seq() const
  {
    return nullptr;
  }

  obj::nil_ptr obj::nil::first() const
  {
    return this;
  }

  obj::nil_ptr obj::nil::next() const
  {
    return nullptr;
  }

  obj::cons_ptr obj::nil::conj(object_ptr const head) const
  {
    return make_box<obj::cons>(head, nullptr);
  }

  obj::nil_ptr obj::nil::next_in_place()
  {
    return nullptr;
  }
}
