#include <bertrand/bertrand.hpp>

void contract_thrower() { Require(false, "Something goes wrong here"); }

void wrapper_one() { contract_thrower(); }

void wrapper_two() { wrapper_one(); }

int main(int, char **) {
  wrapper_two();
  return 0;
}