cmake_minimum_required(VERSION 3.15)

project("bertrand-subdir-example")

add_subdirectory(../../ ${PROJECT_BINARY_DIR}/bertrand-build EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME}
               ${CMAKE_CURRENT_SOURCE_DIR}/../installation-test/src/main.cc)
target_link_libraries(${PROJECT_NAME} bertrand)
