<img align="right" src="bertrand_logo.png">

[![GitHub Releases](https://img.shields.io/github/release/bernedom/bertrand.svg)](https://github.com/bernedom/bertrand/releases)
[![GitHub license](https://img.shields.io/badge/license-LGPL%20v3-blue.svg)](https://raw.githubusercontent.com/bernedom/bertrand/master/LICENSE)
[![Build Status](https://github.com/bernedom/bertrand/workflows/CI/badge.svg?branch=master)](https://github.com/bernedom/bertrand/actions)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/bernedom/bertrand.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/bernedom/bertrand/context:cpp)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/4ed8c3ef2e794d238d289493a31d3b28)](https://www.codacy.com/manual/bernedom/bertrand?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=bernedom/bertrand&amp;utm_campaign=Badge_Grade)

# bertrand
A C++ header only library providing a trivial implementation for design by contract. For a good introduction into design by contract check the [Eiffel Software Explanation](https://www.eiffel.com/values/design-by-contract/introduction/)

An illustrative example:
```cpp

#include <bertrand/bertrand.hpp>

float divide(float dividend, float divisor) {
  Require(divisor != 0, "No division by zero");
  auto result = dividend / divisor;
  Ensure(dividend - result * divisor < std::numeric_limits<float>::epsilon(),
         "Division is correct");
  return result;
}

```

The contract related keywords `Require`, `Ensure` and `Invariant` are implemented. A failing contract results in immediate program termination by an `abort`. The contract message is printed to stderr. For gcc and clang the stack trace is also printed out. Passing multiple values to the contract for additional debug information is possible as long as the type is supported by the stream operator `<<`
```cpp
Require(false, "Something is not right: ", someVariable, " and ", anotherVariable)
```

By default contracts are enabled unless the `NDEBUG` compiler flag is set. Contracts can be force enabled or disabled by passing the compiler flag `BERTRAND_ENABLE_CONTRACTS` or `BERTRAND_DISABLE_CONTRACTS` passing both will lead to a compiler error. 

In order to facilitate testing of the contract functionality contracts are throwing an exception instead of calling abort, if the preprocessor-flag `BERTRAND_CONTRACTS_ARE_EXCEPTIONS` is set. 

By default printing the stacktrace is enabled for gcc or clang, but it can be disabled by passing the preprocessor-flag `BERTRAND_DISABLE_STACKTRACE`.
To enable proper demangling of the symbols of the stack trace pass `-export-dynamic` to the linker. Either directly or by passing `-rdynamic` to the compiler.

## Extended helpers for more complicated tests

Bertrand provides functionality to check if a value is matched against a predefined set of values. For example to check if a raw variable can be casted into an enum. (Casting from an to enums is a code smell, but especially when dealing with data from low-level firmware written in C or assembly this might be required)

```cpp
#include <bertrand/bertrand.hpp>
enum class Color { Red, Green, Blue };

Color toColor(int raw_value) {
  Require(bertrand::find(static_cast<Color>(raw_value))
              .in(Color::Red, Color::Green, Color::Blue),
          "raw_value is a valid color");
  return static_cast<Color>(raw_value);
}

...

toColor(0); // Passes
toColor(5); // Fails with a contract

```

## Building and Installation

To build the unit tests the [Catch2](https://github.com/catchorg/Catch2) unit testing framework is needed. It is retrieved with [conan.io](https://conan.io/)

### cmake installation

```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=${HOME}/local-install
cmake --build . --target install
```
### cmake sub-directory

bertrand can be included using `add_subdirectory` if it cannot be installed into a system. 

If bertrand is included and the `EXCLUDE_FROM_ALL` flag is set, bertrand test can be disabled by setting the `BERTRAND_BUILD_TESTING` option
```cmake
  set(BERTRAND_BUILD_TESTING OFF)
  add_subdirectory(${bertrand_SOURCE_DIR} ${bertrand_BINARY_DIR}
                   EXCLUDE_FROM_ALL)
```

If bertrand is added in a subdirectory and should be installed together with the parent project, set the CMake option `BERTRAND_INSTALL_LIBRARY` to force installation. This applies also if bertrand is pulled using CMakes `FetchContent` or `ExternalProject` 

### conan.io 

bertrand is available from [ConanCenter](https://conan.io/center/)

in the `conanfile.txt` then add

```bash
[requires]
bertrand[>0.0 <2.0]
```
