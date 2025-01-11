#pragma once

#include <jank/runtime/object.hpp>
#include <jank/runtime/obj/cons.hpp>
#include <jank/runtime/core/seq.hpp>
#include <jank/runtime/core/to_string.hpp>

namespace jank::runtime
{
  native_bool equal(object_ptr l, object_ptr r);
}

namespace jank::runtime::obj::detail
{
  template <typename Derived, typename It>
  struct iterator_sequence
  {
    /* NOLINTNEXTLINE(bugprone-crtp-constructor-accessibility) */
    iterator_sequence() = default;

    /* NOLINTNEXTLINE(bugprone-crtp-constructor-accessibility) */
    iterator_sequence(object_ptr const &c, It const &b, It const &e, size_t const s)
      : coll{ c }
      , begin{ b }
      , end{ e }
      , size{ s }
    {
      if(begin == end)
      {
        throw std::runtime_error{ "iterator_sequence for empty sequence" };
      }
    }

    /* behavior::object_like */
    native_bool equal(object const &o) const
    {
      return visit_seqable(
        [this](auto const typed_o) {
          auto seq(typed_o->fresh_seq());
          for(auto it(begin); it != end; ++it, seq = seq->next_in_place())
          {
            if(seq == nullptr || !runtime::equal(*it, seq->first()))
            {
              return false;
            }
          }
          return true;
        },
        []() { return false; },
        &o);
    }

    void to_string(fmt::memory_buffer &buff) const
    {
      runtime::to_string(begin, end, "(", ')', buff);
    }

    native_persistent_string to_string() const
    {
      fmt::memory_buffer buff;
      runtime::to_string(begin, end, "(", ')', buff);
      return native_persistent_string{ buff.data(), buff.size() };
    }

    native_persistent_string to_code_string() const
    {
      fmt::memory_buffer buff;
      runtime::to_code_string(begin, end, "(", ')', buff);
      return native_persistent_string{ buff.data(), buff.size() };
    }

    native_hash to_hash() const
    {
      return hash::ordered(begin, end);
    }

    /* behavior::seqable */
    native_box<Derived> seq()
    {
      return static_cast<Derived *>(this);
    }

    native_box<Derived> fresh_seq() const
    {
      return make_box<Derived>(coll, begin, end, size);
    }

    /* behavior::countable */
    size_t count() const
    {
      return size;
    }

    /* behavior::sequenceable */
    object_ptr first() const
    {
      return *begin;
    }

    native_box<Derived> next() const
    {
      auto n(begin);
      ++n;

      if(n == end)
      {
        return nullptr;
      }

      return make_box<Derived>(coll, n, end, size);
    }

    /* behavior::sequenceable_in_place */
    native_box<Derived> next_in_place()
    {
      ++begin;

      if(begin == end)
      {
        return nullptr;
      }

      return static_cast<Derived *>(this);
    }

    obj::cons_ptr conj(object_ptr const head)
    {
      return make_box<obj::cons>(head, static_cast<Derived *>(this));
    }

    object_ptr coll{};
    /* Not default constructible. */
    It begin, end;
    size_t size{};
  };
}
