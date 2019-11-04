#include <catch.hpp>

#include <bertrand/bertrand.hpp>

TEST_CASE("Assert that tests are run with abort as exceptions") {
#ifndef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
  STATIC_REQUIRE(false);
  /// Tests must be run with #ifndef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS"
#endif
}

TEST_CASE(
    "GIVEN a precondition contract WHEN passed true THEN no assert happens") {

  REQUIRE_NOTHROW(require(true, ""));
}

TEST_CASE(
    "GIVEN a precondition contract WHEN passed false THEN assert happens") {
  REQUIRE_THROWS(require(false, ""));
}

TEST_CASE(
    "GIVEN a postcondition contract WHEN passed true THEN no assert happens") {
  REQUIRE_NOTHROW(ensure(true, ""));
}

TEST_CASE(
    "GIVEN a postcondition contract WHEN passed false THEN  assert happens") {
  REQUIRE_THROWS(ensure(false, ""));
}
TEST_CASE(
    "GIVEN an invariant contract WHEN passed true THEN no assert happens") {
  REQUIRE_NOTHROW(invariant(true, ""));
}

TEST_CASE("GIVEN a invariant contract WHEN passed false THEN  assert happens") {
  REQUIRE_THROWS(invariant(false, ""));
}