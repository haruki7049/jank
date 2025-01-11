#pragma once

#include <iostream>
#include <any>

#include <jank/runtime/object.hpp>
#include <jank/runtime/behavior/callable.hpp>
#include <jank/runtime/behavior/metadatable.hpp>

namespace jank::runtime
{
  namespace obj::detail
  {
    /* This must be constructed with a std::function object, since we need put it into a
     * std::any and pull it out based on how we call it. i.e. if we try to call it with
     * one param, we try to grab it from the std::any as a std::function<object_ptr (object_ptr)>.
     *
     * This means you can't just dump a lambda into this. Build a std::function from it first. */
    struct function_type
    {
      template <typename T>
      using value_type = std::function<T>;

      function_type() = default;

      template <typename R, typename... Args>
      function_type(R (* const f)(Args...))
        : function_type{ value_type<R(Args...)>{ f } }
      {
      }

      template <typename R, typename... Args>
      function_type(value_type<R(Args...)> &&f)
        : value{ std::move(f) }
      {
      }

      template <typename R, typename... Args>
      function_type(value_type<R(Args...)> const &f)
        : value{ f }
      {
      }

      template <typename F>
      F const *get() const
      {
        return std::any_cast<F>(&value);
      }

      std::any value;
    };
  }

  template <size_t Arity>
  struct invalid_arity : std::runtime_error
  {
    invalid_arity(native_persistent_string const &name)
      : std::runtime_error{ fmt::format("invalid call to {} with {} args provided", name, Arity) }
    {
    }
  };

  template <>
  struct static_object<object_type::native_function_wrapper>
    : gc
    , behavior::callable
  {
    static constexpr native_bool pointer_free{ false };

    static_object() = default;
    static_object(static_object &&) = default;
    static_object(static_object const &) = default;
    static_object(obj::detail::function_type &&d);
    static_object(obj::detail::function_type const &d);

    /* behavior::object_like */
    native_bool equal(object const &) const;
    native_persistent_string to_string() const;
    void to_string(fmt::memory_buffer &buff) const;
    native_persistent_string to_code_string() const;
    native_hash to_hash() const;

    /* behavior::callable */
    object_ptr call() final;
    object_ptr call(object_ptr) final;
    object_ptr call(object_ptr, object_ptr) final;
    object_ptr call(object_ptr, object_ptr, object_ptr) final;
    object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr) final;
    object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr, object_ptr) final;
    object_ptr call(object_ptr, object_ptr, object_ptr, object_ptr, object_ptr, object_ptr) final;
    object_ptr
      call(object_ptr, object_ptr, object_ptr, object_ptr, object_ptr, object_ptr, object_ptr)
        final;
    object_ptr call(object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr) final;
    object_ptr call(object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr) final;
    object_ptr call(object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr,
                    object_ptr) final;

    object_ptr this_object_ptr() final;

    /* behavior::metadatable */
    native_box<static_object> with_meta(object_ptr m) const;

    object base{ object_type::native_function_wrapper };
    obj::detail::function_type data{};
    option<object_ptr> meta;
  };

  namespace obj
  {
    using native_function_wrapper = static_object<object_type::native_function_wrapper>;
    using native_function_wrapper_ptr = native_box<native_function_wrapper>;
  }

  namespace detail
  {
    /* TODO: Is this needed, given dynamic_call? */
    template <typename F, typename... Args>
    object_ptr invoke(F const &f, Args &&...args)
    {
      if constexpr(std::is_function_v<std::remove_pointer_t<std::decay_t<decltype(f)>>>)
      {
        return f(std::forward<Args>(args)...);
      }
      else
      {
        auto const * const c((*f)->as_callable());

        if(c)
        {
          return c->call(std::forward<Args>(args)...);
        }
        else
        {
          /* TODO: Better error. */
          std::cout << "(invoke) object is not callable: " << **f << std::endl;
          throw std::runtime_error{ "object is not callable" };
        }
      }
    }
  }
}
