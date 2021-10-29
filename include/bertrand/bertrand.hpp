/**
 * This file is part of "bertrand" version 0.0.18
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
#include <regex>
#include <string>
#include <unistd.h>
#else
#undef BERTRAND_ENABLE_STACKTRACE
#endif
#endif

inline namespace bertrand {

#if defined(BERTRAND_ENABLE_STACKTRACE)
inline void print_stacktrace(std::stringstream &output) {
  output << "Stack trace:\n";

  // storage array for stack trace address data
  void *addrlist[64];

  // retrieve current stack addresses
  int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void *));

  if (addrlen == 0) {
    output << "  Empty stack trace, possibly corrupt\n";
    return;
  }

  // resolve addresses into strings containing "filename(function+address)",
  // this array must be free()-ed
  char **symbollist = backtrace_symbols(addrlist, addrlen);

  // allocate string which will be filled with the demangled function name
  size_t max_name_size = 256;
  char *demangled_name = new char[max_name_size];
  int demangle_status;

  // iterate over the returned symbol lines. skip the first, it is the
  // address of this function.
  // std::regex tokenizer_regex{"(.+)\\((.+)\\+(0x[0-9]+)\\)\\[(.+)\\]" };
  std::regex tokenizer_regex{"(.+)\\((.+)\\+(0x[a-f0-9]+)\\)(.+)"};

  // Skip the first two functions on the stack (this function and
  // bertrand::assert_handler)
  for (int i = 2; i < addrlen; i++) {

    std::smatch matches;
    std::string str(symbollist[i]);
    if (std::regex_search(str, matches, tokenizer_regex)) {
      const std::string module_name = matches[1].str();
      const std::string mangled_name = matches[2].str();
      const std::string offset = matches[3].str();
      const std::string address = matches[4].str();

      char *ret = abi::__cxa_demangle(mangled_name.c_str(), demangled_name,
                                      &max_name_size, &demangle_status);
      if (demangle_status == 0) {
        demangled_name = ret; // use possibly realloc()-ed string
        output << "  " << module_name << " : " << demangled_name << "+"
               << offset << "\n";

      } else {
        // demangling failed. Output function name as a C function with
        // no arguments.
        output << "  " << module_name << " : " << mangled_name << "()+"
               << offset << "\n";
      }
    } else {
      // couldn't parse the line? print the whole line.
      output << "  " << symbollist[i] << "\n";
    }
  }

  delete[] demangled_name;
  delete[] symbollist;
}

#endif

/// struct to check if value is contained in a list of values. i.e. to check
/// if a value is valid for an enum
template <typename T> struct find {

  explicit constexpr find(const T &value) noexcept : value_{value} {}

  template <typename... Args>
  constexpr bool in(const Args &...args) const noexcept {
    return (... || (args == value_));
  }

private:
  const T &value_;
};

template <typename... Args>
inline void assert_handler(bool expr, const char *expression, const char *file,
                           int line, const char *type, const char *message,
                           const Args &...args) {
  if (!expr) {
    std::stringstream buffer;

    buffer << "Assert at: " << file << ":" << line << ": " << type << " ('"
           << expression << "') " << message;
    if constexpr (sizeof...(args) > 0) {
      (buffer << ... << args);
    }
    buffer << "\n";

#ifdef BERTRAND_ENABLE_STACKTRACE

    print_stacktrace(buffer);

#endif
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