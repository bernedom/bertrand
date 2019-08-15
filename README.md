# bertrand
A C++ header only library providing functionalitiy for design by contract

```cpp

#include <bertrand/bertrand.hpp>

int divide(int dividend, int divisor){
  require(divisor != 0);

  return dividend / divisor;
}

```