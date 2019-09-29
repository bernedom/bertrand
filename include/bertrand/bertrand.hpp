#pragma once

///@todo add text
///@todo add file location
///@todo add possibility for delivering stack trace

// asserts as exceptions is a workaround for testing purposes, do not use in
// production
#ifdef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <exception>

namespace bertrand {
inline void assert_handler(bool b) {
  if (!b) {
    throw(std::exception());
  }
}
} // namespace bertrand

#define __bertrand_handle_assert(EXPR) bertrand::assert_handler((EXPR))

#else
#include <cassert>

#define __bertrand_handle_assert(EXPR) assert(EXPR)

#endif

#ifndef NDEBUG
#define require(EXPR, MSG) __bertrand_handle_assert(EXPR)
#define ensure(EXPR, MSG) __bertrand_handle_assert(EXPR)
#define invariant(EXPR, MSG) __bertrand_handle_assert(EXPR)
#else
#define require(EXPR, MSG)
#define ensure(EXPR, MSG)
#define invariant(EXPR, MSG)
#endif