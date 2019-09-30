/**
 * This file is part of "bertrand"
 * A minimalistic, header only implementation of design by contract for C++
 *
 * As a header only library using this in proprietary software is fine as long
 * as it is used unmodified and that there is somewhere a remark about its usage
 * and a link to the lgpl licence
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

///@todo add file location
///@todo add possibility for delivering stack trace

// asserts as exceptions is a workaround for testing purposes, do not use in
// production
#ifdef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <stdexcept>

namespace bertrand {
inline void assert_handler(bool b, const char *message) {
  if (!b) {
    // use sprintf mangling here
    throw(std::runtime_error(message));
  }
}
} // namespace bertrand

#define __bertrand_handle_assert(EXPR, MSG)                                    \
  bertrand::assert_handler((EXPR), MSG)

#else
#include <cassert>

#define __bertrand_handle_assert(EXPR, MSG) assert(EXPR &&MSG)

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