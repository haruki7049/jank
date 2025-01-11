#pragma once

#include <jank/runtime/object.hpp>

namespace jank::runtime::obj
{
  using cons_ptr = native_box<struct cons>;
  using persistent_string_ptr = native_box<struct persistent_string>;
  using persistent_string_sequence_ptr = native_box<struct persistent_string_sequence>;

  struct persistent_string_sequence : gc
  {
    static constexpr object_type obj_type{ object_type::persistent_string_sequence };
    static constexpr native_bool pointer_free{ false };
    static constexpr native_bool is_sequential{ true };

    persistent_string_sequence() = default;
    persistent_string_sequence(persistent_string_sequence &&) noexcept = default;
    persistent_string_sequence(persistent_string_sequence const &) = default;
    persistent_string_sequence(obj::persistent_string_ptr const s);
    persistent_string_sequence(obj::persistent_string_ptr const s, size_t const i);

    /* behavior::object_like */
    native_bool equal(object const &) const;
    void to_string(util::string_builder &buff) const;
    native_persistent_string to_string() const;
    native_persistent_string to_code_string() const;
    native_hash to_hash() const;

    /* behavior::countable */
    size_t count() const;

    /* behavior::seqable */
    persistent_string_sequence_ptr seq();
    persistent_string_sequence_ptr fresh_seq() const;

    /* behavior::sequenceable */
    object_ptr first() const;
    persistent_string_sequence_ptr next() const;
    obj::cons_ptr conj(object_ptr head);

    /* behavior::sequenceable_in_place */
    persistent_string_sequence_ptr next_in_place();

    object base{ obj_type };
    obj::persistent_string_ptr str{};
    size_t index{};
  };
}
