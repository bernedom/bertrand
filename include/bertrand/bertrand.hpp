/**
 * This file is part of "bertrand"
 * https://github.com/bernedom/bertrand
 * A minimalistic, header only implementation of design by contract for C++
 *
 * As a header only library using this in proprietary software is fine as long
 * as it is used unmodified and that there is somewhere a remark about its usage
 * and a link to the lgpl licence
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

///@todo add function to message
///@todo add possibility for delivering stack trace

#ifndef NDEBUG
#include <iostream>
#include <sstream>
// asserts as exceptions is a workaround for testing purposes, do not use in
// production
#ifdef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <stdexcept>
#else
#include <cstdlib>
#endif

namespace bertrand {
inline void assert_handler(bool expr, const char *expression,
                           const char *message, const char *file, int line) {
  if (!expr) {
    std::stringstream buffer;
    buffer << file << ":" << line << ": ('" << expression << "') " << message
           << "\n";
    std::cerr << buffer.str();
#ifdef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
    throw std::runtime_error(buffer.str());
#else
    abort();
#endif
  }
}
} // namespace bertrand

/// this forwarding macro is needed to get the correct file and line information
#define __bertrand_handle_assert(EXPR, MSG, FILE, LINE)                        \
  bertrand::assert_handler((EXPR), #EXPR, MSG, FILE, LINE)

#define require(EXPR, MSG)                                                     \
  __bertrand_handle_assert(EXPR, MSG, __FILE__, __LINE__)
#define ensure(EXPR, MSG)                                                      \
  __bertrand_handle_assert(EXPR, MSG, __FILE__, __LINE__)
#define invariant(EXPR, MSG)                                                   \
  __bertrand_handle_assert(EXPR, MSG, __FILE__, __LINE__)
#else
#define require(EXPR, MSG) (static_cast<void>(0))
#define ensure(EXPR, MSG) (static_cast<void>(0))
#define invariant(EXPR, MSG) (static_cast<void>(0))
#endif