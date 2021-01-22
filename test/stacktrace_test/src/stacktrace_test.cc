
#define CATCH_CONFIG_MAIN /* This tells Catch to provide a main() - only do    \
                           this in one cpp file*/
#include <bertrand/bertrand.hpp>

#include <catch2/catch.hpp>
#include <sstream>
// Test scaffolding are three nested functions
void contract_thrower() { Require(false, "Something goes wrong here"); }

void wrapper_one() { contract_thrower(); }

void wrapper_two() { wrapper_one(); }
// End scaffolding

TEST_CASE("GIVEN a contract AND printing stacktrace is enable WHEN the "
          "contract fails THEN the "
          "stack trace is printed out") {
#if !defined(BERTRAND_ENABLE_STACKTRACE)
  static_assert(
      false,
      "Stack tracing is not neabled, enable with BERTRAND_ENABLE_STACKTRACE");
#endif

  REQUIRE_THROWS_WITH(wrapper_two(),
                      Catch::Matchers::Contains("contract_thrower()") &&
                          Catch::Matchers::Contains("wrapper_one()") &&
                          Catch::Matchers::Contains("wrapper_two()"));
}

// Test scaffolding are three nested functions
void contract_thrower(int i) { Require(i == 0, "1 is not 0"); }

void wrapper_one(int i) { contract_thrower(i); }

void wrapper_two(int i) { wrapper_one(i); }
// End scaffolding

TEST_CASE("GIVEN a contract AND printing stacktrace is enabled WHEN the "
          "contract fails THEN the "
          "stack trace is printed out AND argument types are in stacktrace") {

  REQUIRE_THROWS_WITH(wrapper_two(1),
                      Catch::Matchers::Contains("contract_thrower(int)") &&
                          Catch::Matchers::Contains("wrapper_one(int)") &&
                          Catch::Matchers::Contains("wrapper_two(int)"));
}
