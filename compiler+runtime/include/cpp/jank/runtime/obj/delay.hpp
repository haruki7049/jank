#pragma once

namespace jank::runtime
{
  template <>
  struct static_object<object_type::delay> : gc
  {
    static constexpr native_bool pointer_free{ false };

    static_object() = default;
    static_object(object_ptr fn);

    /* behavior::object_like */
    native_bool equal(object const &) const;
    native_persistent_string to_string() const;
    void to_string(fmt::memory_buffer &buff) const;
    native_persistent_string to_code_string() const;
    native_hash to_hash() const;

    /* behavior::derefable */
    object_ptr deref();

    object base{ object_type::delay };
    object_ptr val{};
    object_ptr fn{};
    object_ptr error{};
    std::mutex mutex;
  };

  namespace obj
  {
    using delay = static_object<object_type::delay>;
    using delay_ptr = native_box<delay>;
  }
}
