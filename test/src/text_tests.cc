#include <catch.hpp>

#define __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <bertrand/bertrand.hpp>

TEST_CASE("GIVEN a contract with a message WHEN the contract fails THEN the "
          "message is passed on") {
  REQUIRE_THROWS(require(false, "Cannot be false"));
}