#include <catch.hpp>

#include <iostream>
#include <limits>
#include <sstream>

#include <bertrand/bertrand.hpp>

namespace {
float divide(float dividend, float divisor) {
  Require(divisor != 0, "No division by zero");
  auto result = dividend / divisor;
  Ensure(dividend - result * divisor < std::numeric_limits<float>::epsilon(),
         "Division is correct");
  return result;
}
} // namespace
TEST_CASE("GIVEN a contract with a message WHEN the contract fails THEN the "
          "message is passed on") {
  std::stringstream expected;
  expected << __FILE__ << ":";
  expected << __LINE__ + 1 << ": ('false') Cannot be false\n";
  REQUIRE_THROWS_WITH(Require(false, "Cannot be false"), expected.str());
}

TEST_CASE(
    "GIVEN an example function WHEN executed THEN no contract is triggered") {
  REQUIRE_NOTHROW(divide(10.0f, 5.0f));
}

TEST_CASE("GIVEN an example function WHEN executed with invalid arguments THEN "
          "contract is triggered") {
  REQUIRE_THROWS(divide(10.0f, 0.0f));
}

TEST_CASE("GIVEN an example function WHEN executed with invalid arguments THEN "
          "contract is triggered AND message is set with correct location") {
  std::stringstream expected;
  expected << __FILE__ << ":";
  expected << 11 << ": ('divisor != 0') No division by zero\n";
  REQUIRE_THROWS_WITH(divide(10.0f, 0.0f), expected.str());
}

TEST_CASE("GIVEN a contract WHEN it fails THEN message is printed to stderr") {
  std::stringstream new_stderr;
  auto old_stderr = std::cerr.rdbuf(new_stderr.rdbuf());
  std::stringstream expected;
  expected << __FILE__ << ":";
  expected << __LINE__ + 1 << ": ('false') Cannot be false\n";
  REQUIRE_THROWS_WITH(Require(false, "Cannot be false"), expected.str());
  REQUIRE(expected.str().compare(new_stderr.str()) == 0);
  std::cerr.rdbuf(old_stderr);
}
