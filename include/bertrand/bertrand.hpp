#pragma once

///@todo add text
///@todo add file location
///@todo add possibility for delivering stack trace

// asserts as exceptions is a workaround for testing purposes, do not use in
// production
#ifdef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <stdexcept>

namespace bertrand {
inline void assert_handler(bool b, const char *message) {
  if (!b) {
    throw(std::runtime_error(message));
  }
}
} // namespace bertrand

#define __bertrand_handle_assert(EXPR, MSG)                                    \
  bertrand::assert_handler((EXPR), MSG)

#else
#include <cassert>

#define __bertrand_handle_assert(EXPR, MSG) assert(EXPR)

#endif

#ifndef NDEBUG
#define require(EXPR, MSG) __bertrand_handle_assert(EXPR, MSG)
#define ensure(EXPR, MSG) __bertrand_handle_assert(EXPR, MSG)
#define invariant(EXPR, MSG) __bertrand_handle_assert(EXPR, MSG)
#else
#define require(EXPR, MSG)
#define ensure(EXPR, MSG)
#define invariant(EXPR, MSG)
#endif