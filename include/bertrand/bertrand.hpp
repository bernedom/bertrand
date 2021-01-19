/**
 * This file is part of "bertrand" version 0.0.14
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

///@todo add possibility for delivering stack trace for macos and msvc
/// @todo remove calls to bertrand from stack

// fail compilation of contracts are force-enabled and force-disabled at the
// same time
#if defined(BERTRAND_DISABLE_CONTRACTS) && defined(BERTRAND_ENABLE_CONTRACTS)
static_assert(false,
              "Cannot force enable and disable contracts at the same time");
#endif

#if !defined(NDEBUG) && !defined(BERTRAND_ENABLE_CONTRACTS) &&                 \
    !defined(BERTRAND_DISABLE_CONTRACTS)
#define BERTRAND_ENABLE_CONTRACTS
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

#if defined(BERTRAND_DISABLE_STACKTRACE) && defined(BERTRAND_ENABLE_STACKTRACE)
static_assert(false, "Cannot enable and disable stacktrace at the same time");
#endif

#if !defined(BERTRAND_DISABLE_STACKTRACE)
#if __has_include(<execinfo.h>)
#define BERTRAND_ENABLE_STACKTRACE
#include <cxxabi.h>

#include <dlfcn.h> // for dladdr
#include <execinfo.h>
#include <string>
#include <unistd.h>

static inline void print_stacktrace() {
  std::cerr << "Stack trace\n";

  // storage array for stack trace address data
  void *addrlist[64];

  // retrieve current stack addresses
  int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void *));

  if (addrlen == 0) {
    std::cerr << "  Empty stack trace, possibly corrupt\n";
    return;
  }

  // resolve addresses into strings containing "filename(function+address)",
  // this array must be free()-ed
  char **symbollist = backtrace_symbols(addrlist, addrlen);

  // allocate string which will be filled with the demangled function name
  size_t funcnamesize = 256;
  char *funcname = (char *)malloc(funcnamesize);

  // iterate over the returned symbol lines. skip the first, it is the
  // address of this function.
  for (int i = 1; i < addrlen; i++) {
    char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

    // find parentheses and +address offset surrounding the mangled name:
    // ./module(function+0x15c) [0x8048a6d]
    // todo use <regex>
    for (char *p = symbollist[i]; *p; ++p) {
      if (*p == '(')
        begin_name = p;
      else if (*p == '+')
        begin_offset = p;
      else if (*p == ')' && begin_offset) {
        end_offset = p;
        break;
      }
    }

    if (begin_name && begin_offset && end_offset && begin_name < begin_offset) {
      *begin_name++ = '\0';
      *begin_offset++ = '\0';
      *end_offset = '\0';

      // mangled name is now in [begin_name, begin_offset) and caller
      // offset in [begin_offset, end_offset). now apply
      // __cxa_demangle():

      int status;
      char *ret =
          abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
      if (status == 0) {
        funcname = ret; // use possibly realloc()-ed string
        std::cerr << "  " << symbollist[i] << " : " << funcname << "+"
                  << begin_offset << "\n";

      } else {
        // demangling failed. Output function name as a C function with
        // no arguments.
        std::cerr << "  " << symbollist[i] << " : " << begin_name << "()+"
                  << begin_offset << "\n";
      }
    } else {
      // couldn't parse the line? print the whole line.
      std::cerr << "  " << symbollist[i] << "\n";
    }
  }

  free(funcname);
  free(symbollist);
}
#else
#undef BERTRAND_ENABLE_STACKTRACE
#endif
#endif

namespace bertrand {

/// struct to check if value is contained in a list of values. i.e. to check if
/// a value is valid for an enum
template <typename T> struct find {

  explicit constexpr find(const T &value) noexcept : value_{value} {}

  template <typename... Args>
  constexpr bool in(const Args &... args) const noexcept {
    return (... || (args == value_));
  }

private:
  const T &value_;
};

template <typename... Args>
inline void assert_handler(bool expr, const char *expression, const char *file,
                           int line, const char *type, const char *message,
                           const Args &... args) {
  if (!expr) {
    std::stringstream buffer;

    buffer << "Assert at: " << file << ":" << line << ": " << type << " ('"
           << expression << "') " << message;
    if constexpr (sizeof...(args) > 0) {
      (buffer << ... << args);
    }
    buffer << "\n";
    std::cerr << buffer.str();

#ifdef BERTRAND_ENABLE_STACKTRACE
    void *stack[10];
    size_t size = backtrace(stack, 10);

    backtrace_symbols_fd(stack, size, STDERR_FILENO);

    print_stacktrace();

#endif
#ifdef BERTRAND_CONTRACTS_ARE_EXCEPTIONS
    throw std::runtime_error(buffer.str());
#else
    abort();
#endif
  }
}
} // namespace bertrand

/// this forwarding macro is needed to get the correct file and line information
#define bertrand_handle_assert_impl(EXPR, FILE, LINE, CONTRACT_TYPE, ...)      \
  bertrand::assert_handler((EXPR), #EXPR, FILE, LINE, CONTRACT_TYPE,           \
                           __VA_ARGS__)

#define Require(EXPR, ...)                                                     \
  bertrand_handle_assert_impl(EXPR, __FILE__, __LINE__, "Require", __VA_ARGS__)
#define Ensure(EXPR, ...)                                                      \
  bertrand_handle_assert_impl(EXPR, __FILE__, __LINE__, "Ensure", __VA_ARGS__)
#define Invariant(EXPR, ...)                                                   \
  bertrand_handle_assert_impl(EXPR, __FILE__, __LINE__, "Invariant",           \
                              __VA_ARGS__)
#else
#define Require(EXPR, ...) (static_cast<void>(0))
#define Ensure(EXPR, ...) (static_cast<void>(0))
#define Invariant(EXPR, ...) (static_cast<void>(0))
#endif