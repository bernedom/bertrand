cmake_minimum_required(VERSION 3.15)

project("bertrand-Example-conan")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(bertrand CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
               ${CMAKE_CURRENT_SOURCE_DIR}/../installation-test/src/main.cc)
target_link_libraries(${PROJECT_NAME} bertrand::bertrand)
