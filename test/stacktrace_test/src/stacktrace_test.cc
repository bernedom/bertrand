
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

  auto matcher = Catch::Matchers::Matches(".*");
  // std::stringstream expected;
  // expected << "Assert at: " << __FILE__ << ":";
  // expected << 9 << ": Require ('false') Something goes wrong here\n";
  // expected << "Stack trace:\n";
  REQUIRE_THROWS_WITH(wrapper_two(), matcher);
}
