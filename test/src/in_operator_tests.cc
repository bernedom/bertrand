#include <catch2/catch.hpp>

#include <bertrand/bertrand.hpp>

#include <vector>

TEST_CASE("GIVEN_a_value_WHEN_tested_against_a_list_of_values_AND_list_"
          "contains_searched_value_THEN_result_is_true") {

  REQUIRE_NOTHROW(Require(bertrand::find(0).in(0, 1, 2), "0 is in 0,1,2"));
}

TEST_CASE("GIVEN_a_value_WHEN_tested_against_a_list_of_values_AND_list_does_not"
          "contain_searched_value_THEN_result_is_false") {

  REQUIRE_THROWS(Require(bertrand::find(0).in(1, 2), "0 is in 1,2"));
}
