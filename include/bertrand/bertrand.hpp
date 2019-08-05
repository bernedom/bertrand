#pragma once

#ifdef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <exception>

void assert_handler(bool b) {
  if (!b) {
    throw(std::exception());
  }
}

#define __bertrand_handle_assert(ARG) assert_handler((ARG))

#else
#include <cassert>

#define __bertrand_handle_assert(ARG) assert(ARG)

#endif

#define require(ARG) __bertrand_handle_assert(ARG)
