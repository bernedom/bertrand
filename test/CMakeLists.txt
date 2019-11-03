cmake_minimum_required(VERSION 3.12)

add_executable(bertrand_tests)
target_sources(bertrand_tests
               PRIVATE
               src/Catch2Main.cc
               src/text_tests.cc
               src/bertrand_tests.cc)

target_include_directories(
  bertrand_tests
  PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
          $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

target_compile_features(bertrand_tests PRIVATE cxx_std_17)

target_compile_options(
  bertrand_tests
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
    $<$<CXX_COMPILER_ID:MSVC>:/D__BERTRAND_CONTRACTS_ARE_EXCEPTIONS>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Werror>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-pedantic>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-D__BERTRAND_CONTRACTS_ARE_EXCEPTIONS>
    $<$<CXX_COMPILER_ID:AppleClang>:-D__BERTRAND_CONTRACTS_ARE_EXCEPTIONS>
    $<$<CXX_COMPILER_ID:AppleClang>:-Werror>
    $<$<CXX_COMPILER_ID:AppleClang>:-pedantic>
    $<$<CXX_COMPILER_ID:AppleClang>:-Wall>)

add_executable(failing_contract)
target_sources(failing_contract PRIVATE src/failing_contract.cc)

target_include_directories(
  failing_contract
  PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
          $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

add_test(NAME bertrand_tests COMMAND bertrand_tests)
