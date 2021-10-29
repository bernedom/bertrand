cmake_minimum_required(VERSION 3.15)

project(
  "bertrand"
  VERSION 0.0.18
  DESCRIPTION
    "A header only c++ library providing functionality for design by contract"
  HOMEPAGE_URL "https://github.com/bernedom/bertrand")

include(GNUInstallDirs)
include(CTest)

if(CMAKE_PROJECT_NAME STREQUAL ${PROJECT_NAME})
  set(BERTRAND_IS_ROOT_PROJECT TRUE)
else()
  set(BERTRAND_IS_ROOT_PROJECT FALSE)
endif()

option(BERTRAND_BUILD_TESTING
       "enable testing for bertrand; Requires global BUILD_TESTING to be set"
       ${BERTRAND_IS_ROOT_PROJECT})
option(
  BERTRAND_INSTALL_LIBRARY
  "Set to ON to install bertrand; default is disabled if bertrand is included using add_subdirectory"
  ${BERTRAND_IS_ROOT_PROJECT})

add_library(${PROJECT_NAME} INTERFACE)
# add alias so the project can be uses with add_subdirectory
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Adding the install interface generator expression makes sure that the include
# files are installed to the proper location (provided by GNUInstallDirs)
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

if(BUILD_TESTING AND BERTRAND_BUILD_TESTING)
  add_subdirectory(test)
endif()

if(BERTRAND_INSTALL_LIBRARY)
  # locations are provided by GNUInstallDirs
  install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

  configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(
    EXPORT ${PROJECT_NAME}_Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/bertrand
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

  include(CPack)
endif()
