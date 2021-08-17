cmake_minimum_required(VERSION 3.15)

project("Bertrand-Install-Test")

find_package(bertrand CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cc)
target_link_libraries(${PROJECT_NAME} bertrand::bertrand)
