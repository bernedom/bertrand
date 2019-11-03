#include <catch.hpp>
#include <cstdint>
#include <limits>
#include <sstream>

#define __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <bertrand/bertrand.hpp>

namespace {
float divide(float dividend, float divisor) {
  require(divisor != 0, "No division by zero");
  auto result = dividend / divisor;
  ensure(dividend - result * divisor < std::numeric_limits<float>::epsilon(),
         "Division is correct");
  return result;
}
} // namespace
TEST_CASE("GIVEN a contract with a message WHEN the contract fails THEN the "
          "message is passed on") {
  std::stringstream expected;
  expected << __FILE__ << ":";
  expected << __LINE__ + 1 << ": ('false') Cannot be false\n";
  REQUIRE_THROWS_WITH(require(false, "Cannot be false"), expected.str());
}

TEST_CASE(
    "GIVEN an example function WHEN executed THEN no contract is triggered") {
  REQUIRE_NOTHROW(divide(10.0f, 5.0f));
}

TEST_CASE("GIVEN an example function WHEN executed with invalid arguments THEN "
          "contract is triggered") {
  REQUIRE_THROWS(divide(10.0f, 0.0f));
}
