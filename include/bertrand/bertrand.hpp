#pragma once

// asserts as exceptions is a workaround for testing purposes, do not use in
// production
#ifdef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <exception>

namespace bertrand {
void assert_handler(bool b) {
  if (!b) {
    throw(std::exception());
  }
}
} // namespace bertrand

#define __bertrand_handle_assert(ARG) bertrand::assert_handler((ARG))

#else
#include <cassert>

#define __bertrand_handle_assert(ARG) assert(ARG)

#endif

#define require(ARG) __bertrand_handle_assert(ARG)
#define ensure(ARG) __bertrand_handle_assert(ARG)
#define invariant(ARG) __bertrand_handle_assert(ARG)