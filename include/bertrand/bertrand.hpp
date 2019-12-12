/**
 * This file is part of "bertrand" version 0.0.6
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
#define BERTRAND_ENABLE_CONTRACTS
#endif

#if defined(BERTRAND_DISABLE_CONTRACTS) && defined(BERTRAND_ENABLE_CONTRACTS)
#undef BERTRAND_ENABLE_CONTRACTS
#endif

#ifdef BERTRAND_ENABLE_CONTRACTS
#include <iostream>
#include <sstream>
// asserts as exceptions is a workaround for testing purposes, do not use in
// production
#ifdef BERTRAND_CONTRACTS_ARE_EXCEPTIONS
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
#ifdef BERTRAND_CONTRACTS_ARE_EXCEPTIONS
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

#define Require(EXPR, MSG)                                                     \
  __bertrand_handle_assert(EXPR, MSG, __FILE__, __LINE__)
#define Ensure(EXPR, MSG)                                                      \
  __bertrand_handle_assert(EXPR, MSG, __FILE__, __LINE__)
#define Invariant(EXPR, MSG)                                                   \
  __bertrand_handle_assert(EXPR, MSG, __FILE__, __LINE__)
#else
#define Require(EXPR, MSG) (static_cast<void>(0))
#define Ensure(EXPR, MSG) (static_cast<void>(0))
#define Invariant(EXPR, MSG) (static_cast<void>(0))
#endif