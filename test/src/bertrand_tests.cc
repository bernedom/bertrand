#include <catch.hpp>

#define __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <bertrand/bertrand.hpp>

TEST_CASE(
    "GIVEN a contract-precondition WHEN passed true THEN no assert happens") {

  REQUIRE_NOTHROW(require(true));
}

TEST_CASE(
    "GIVEN a contract precondition WHEN passed false THEN assert happens") {
  REQUIRE_THROWS(require(false));
}