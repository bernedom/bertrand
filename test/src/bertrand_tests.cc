#include <catch2/catch.hpp>

#include <bertrand/bertrand.hpp>

TEST_CASE("Assert that tests are run with abort as exceptions") {
#ifndef BERTRAND_CONTRACTS_ARE_EXCEPTIONS
  STATIC_REQUIRE(false);
  /// Tests must be run with #ifndef BERTRAND_CONTRACTS_ARE_EXCEPTIONS"
#endif
}

TEST_CASE(
    "GIVEN a precondition contract WHEN passed true THEN no assert happens") {

  REQUIRE_NOTHROW(Require(true, ""));
}

TEST_CASE(
    "GIVEN a precondition contract WHEN passed false THEN assert happens") {
  REQUIRE_THROWS(Require(false, ""));
}

TEST_CASE(
    "GIVEN a postcondition contract WHEN passed true THEN no assert happens") {
  REQUIRE_NOTHROW(Ensure(true, ""));
}

TEST_CASE(
    "GIVEN a postcondition contract WHEN passed false THEN  assert happens") {
  REQUIRE_THROWS(Ensure(false, ""));
}
TEST_CASE(
    "GIVEN an Invariant contract WHEN passed true THEN no assert happens") {
  REQUIRE_NOTHROW(Invariant(true, ""));
}

TEST_CASE("GIVEN a Invariant contract WHEN passed false THEN  assert happens") {
  REQUIRE_THROWS(Invariant(false, ""));
}