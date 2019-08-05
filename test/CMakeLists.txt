cmake_minimum_required(VERSION 3.12)

add_executable(bertrand_tests)
target_sources(bertrand_tests
               PRIVATE
               src/Catch2Main.cc
               )

target_include_directories(
  bertrand_tests
  PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
          $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

target_compile_features(bertrand_tests PRIVATE cxx_std_17)

target_compile_options(
  bertrand_tests
  PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
          $<$<CXX_COMPILER_ID:MSVC>:/W4>
          $<$<CXX_COMPILER_ID:MSVC>:/WX>
          $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>
          $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Werror>
          $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-pedantic>)

add_test(NAME bertrand_tests COMMAND bertrand_tests)