# Changelog

## 0.0.13

*   Add function to search for a value in a list of values that are known at compile time
*   Update Catch2 to version 2.13.1
*   Update documentation with hint about ConanCenter

## 0.0.12 

*   Contract type "Require", "Ensure" or "Invariant" is included in the contract-message
*   conanfile.py pulls version out of CMakeLists
*   Linting conanfile.py, cmake object is only generated once

## 0.0.11

*   If bertrand is built from within conan the conanbuildinfo provided by conan will be used instead of getting it from github

## 0.0.10

*   Including bertrand as a sub directory in cmake disables building the tests
*   Catch2 is retrieved with conan and no longer directly in the source tree

## 0.0.9

*   Contracts take variable number of arguments for the contract-message
*   Add logo and repository card 

## 0.0.8

*   Add cmake option `BERTRAND_BUILD_TESTING` (default `ON`) to disable bertrand tests if used with `add_subdirectory`

## 0.0.7

*   Removing double underscores
*   Unit-tests work in release mode as well

## 0.0.6

*   Compiler flag `BERTRAND_ENABLE_CONTRACTS` and `BERTRAND_DISABLE_CONTRACTS` for forced enabled/disabled contracts. 
*   Cmake linting

## 0.0.5

*   Added changelog
*   Removed double underscores from macro that enables exceptions as contracts
*   Require, Ensure, Invariant start with a capital letter