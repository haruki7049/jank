#pragma once

#include <jank/runtime/object.hpp>
#include <jank/runtime/detail/type.hpp>

namespace jank::runtime::obj
{
  using transient_sorted_map_ptr = native_box<struct transient_sorted_map>;

  struct transient_sorted_map : gc
  {
    static constexpr object_type obj_type{ object_type::transient_sorted_map };
    static constexpr bool pointer_free{ false };

    using value_type = runtime::detail::native_transient_sorted_map;
    using persistent_type_ptr = native_box<struct persistent_sorted_map>;

    transient_sorted_map() = default;
    transient_sorted_map(transient_sorted_map &&) noexcept = default;
    transient_sorted_map(transient_sorted_map const &) = default;
    transient_sorted_map(runtime::detail::native_persistent_sorted_map const &d);
    transient_sorted_map(runtime::detail::native_persistent_sorted_map &&d);
    transient_sorted_map(value_type &&d);

    static transient_sorted_map_ptr empty();

    /* behavior::object_like */
    native_bool equal(object const &) const;
    native_persistent_string to_string() const;
    void to_string(util::string_builder &buff) const;
    native_persistent_string to_code_string() const;
    native_hash to_hash() const;

    /* behavior::countable */
    size_t count() const;

    /* behavior::associatively_readable */
    object_ptr get(object_ptr const key) const;
    object_ptr get(object_ptr const key, object_ptr const fallback) const;
    object_ptr get_entry(object_ptr key) const;
    native_bool contains(object_ptr key) const;

    /* behavior::associatively_writable_in_place */
    transient_sorted_map_ptr assoc_in_place(object_ptr const key, object_ptr const val);
    transient_sorted_map_ptr dissoc_in_place(object_ptr const key);

    /* behavior::conjable_in_place */
    transient_sorted_map_ptr conj_in_place(object_ptr head);

    /* behavior::persistentable */
    persistent_type_ptr to_persistent();

    /* behavior::callable */
    object_ptr call(object_ptr) const;
    object_ptr call(object_ptr, object_ptr) const;

    void assert_active() const;

    object base{ obj_type };
    value_type data;
    mutable native_hash hash{};
    native_bool active{ true };
  };
}
