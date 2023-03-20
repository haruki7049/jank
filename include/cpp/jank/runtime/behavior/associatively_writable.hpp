#pragma once

#include <jank/native_box.hpp>

namespace jank::runtime::behavior
{
  struct associatively_writable : virtual gc
  {
    virtual ~associatively_writable() = default;
    virtual object_ptr assoc(object_ptr key, object_ptr val) const = 0;
  };
}