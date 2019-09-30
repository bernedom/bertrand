[![Build Status](https://travis-ci.com/bernedom/bertrand.svg?branch=master)](https://travis-ci.com/bernedom/bertrand)
[![GitHub Releases](https://img.shields.io/github/release/bernedom/betrand.svg)](https://github.com/bernedom/bertrand/releases)
[![GitHub license](https://img.shields.io/badge/license-LGPL%20v3-blue.svg)](https://raw.githubusercontent.com/bernedom/bertrand/master/LICENSE)

# bertrand
A C++ header only library providing a trivial implementation for design by contract

```cpp

#include <bertrand/bertrand.hpp>

int divide(float dividend, float divisor){
  require(divisor != 0);
  auto result = dividend / divisor;
  ensure((result * divisor) < )
  return ;
}

```

## Installation

### cmake

```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=${HOME}/local-install
cmake --build . --target install
```

### conan.io 

bertrand is available from bintray

```bash
conan remote add bernedom https://api.bintray.com/conan/bernedom/conan
```

in the `conanfile.txt` then add

```
[requires]
bertrand[>0.0 <2.0]@bertrand/stable
```