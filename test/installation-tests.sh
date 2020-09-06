#!/bin/bash
# file: test/installation-tests.sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
ROOT_DIR=$(realpath "${SCRIPT_DIR}/../")

setUp(){
    BERTRAND_BUILD_DIR=$(mktemp -d)
    INSTALL_PATH=$(mktemp -d)
    BUILD_DIR=$(mktemp -d)
}

tearDown(){
    if [ -d "${BERTRAND_BUILD_DIR}" ]; then
        rm -rf "${BERTRAND_BUILD_DIR}"
    fi
    
    if [ -d "${INSTALL_PATH}" ]; then
        rm -rf "${INSTALL_PATH}"
    fi
    
    if [ -d "${BUILD_DIR}" ]; then
        rm -rf "${BUILD_DIR}"
    fi
}

testVersionNumberConsistency()
{
    ORIG_DIR=$(pwd)
    cmake "${ROOT_DIR}" -B"${BERTRAND_BUILD_DIR}" -DBUILD_TESTING=off -G Ninja > /dev/null
    cd "${BERTRAND_BUILD_DIR}"
    CMAKE_VERSION=$(cmake --system-information|grep -E "VERSION:STATIC"|grep -E -o '[0-9]+\.[0-9]+\.[0-9]+')
    cd "${ORIG_DIR}"
    GIT_VERSION_EXACT=$(git describe --tags | grep -E -o '^[0-9]+\.[0-9]+\.[0-9]+$')
    CHANGELOG_VERSION=$(sed -n -E '/## [0-9]+\.[0-9]+\.[0-9]+/p' "${ROOT_DIR}/CHANGELOG.md" | head -1 | grep -E -o '[0-9]+\.[0-9]+\.[0-9]+')
    
    assertEquals "version in changelog (${CHANGELOG_VERSION}) does not match cmake version (${CMAKE_VERSION})" "${CHANGELOG_VERSION}" "${CMAKE_VERSION}"
    
    if [ "${GIT_VERSION_EXACT}" != "" ]; then
        assertEquals "version in files (${CMAKE_VERSION}) does not match git tag for release  (${GIT_VERSION_EXACT})" "${CMAKE_VERSION}" "${GIT_VERSION_EXACT}"
    else
        GIT_VERSION=$(git describe --tags | grep -E -o '^[0-9]+\.[0-9]+\.[0-9]+')
        assertNotEquals "version in files (${CMAKE_VERSION}) matches already existing release (${GIT_VERSION}):" "${CMAKE_VERSION}" "${GIT_VERSION}"
    fi
    
    for F in $(find "${ROOT_DIR}/include/bertrand" -name "*.hpp"); do
        FILE_VERSION=$(grep -E 'version [0-9]+\.[0-9]+\.[0-9]+' "${F}" | grep -E -o '[0-9]+\.[0-9]+\.[0-9]+')
        assertEquals "version in header file ${F} does not match cmake version" "${CMAKE_VERSION}" "${FILE_VERSION}"
    done
}

testPureCmakeInstallation(){
    # install bertrand
    cmake "${ROOT_DIR}" -B"${BERTRAND_BUILD_DIR}" -DCMAKE_INSTALL_PREFIX:PATH="${INSTALL_PATH}" -DBUILD_TESTING=off -G Ninja > /dev/null
    cmake --build "${BERTRAND_BUILD_DIR}" --config Release --target install
    assertEquals "Installation build successful" 0 $?
    cmake "${ROOT_DIR}/test/installation-test" -B"${BUILD_DIR}" -DCMAKE_INSTALL_PREFIX:PATH="${INSTALL_PATH}" -G Ninja > /dev/null
    cmake --build "${BUILD_DIR}"
    assertEquals "build against installation successful" 0 $?
    
}

testCpackInstallation(){
    # install bertrand
    cmake "${ROOT_DIR}" -B"${BERTRAND_BUILD_DIR}" -DCPACK_PACKAGE_FILE_NAME=install-bertrand -DBUILD_TESTING=off -G Ninja > /dev/null
    cmake --build "${BERTRAND_BUILD_DIR}" --config Release --target package
    assertEquals "Installation build successful" 0 $?
    "${BERTRAND_BUILD_DIR}/install-bertrand.sh" --prefix="${INSTALL_PATH}" --skip-license --exclude-subdir
    assertEquals "Installation script successful" 0 $?
    
    cmake "${ROOT_DIR}/test/installation-test" -B"${BUILD_DIR}" -DCMAKE_INSTALL_PREFIX:PATH="${INSTALL_PATH}" -G Ninja
    cmake --build "${BUILD_DIR}"
    assertEquals "build against installation successful" 0 $?
    
}

testConanInstallation()
{
    
    # preliminary cleanup
    conan remove -f *@bertrand/testing
    
    conan create "${ROOT_DIR}" bertrand/testing
    assertEquals "Conan installation build successful" 0 $?
    conan install -if "${BUILD_DIR}" "${ROOT_DIR}/test/conan-installation-test"
    assertEquals "Conan installation successful" 0 $?
    
    cmake "${ROOT_DIR}/test/conan-installation-test" -B"${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release -G Ninja > /dev/null
    cmake --build "${BUILD_DIR}"
    assertEquals "build against installation successful" 0 $?
    
    # cleanup
    conan remove -f *@bertrand/testing
}

testDisablingOfBertrandBuildingTests()
{
    cmake "${ROOT_DIR}" -B"${BERTRAND_BUILD_DIR}" -DCMAKE_INSTALL_PREFIX:PATH="${INSTALL_PATH}" -DBUILD_TESTING=on -DBERTRAND_BUILD_TESTING=off -G Ninja
    cmake --build "${BERTRAND_BUILD_DIR}" --config Release
    CURRENT_DIR=$(pwd)
    cd "${BERTRAND_BUILD_DIR}"
    NUMBER_OF_TESTS=$(ctest -N -o | grep -E '^Total Tests: [0-9]+')
    cd "${CURRENT_DIR}"
    assertEquals "No tests found" "Total Tests: 0" "${NUMBER_OF_TESTS}"
}

testGlobalDisablingOfTests()
{
    cmake "${ROOT_DIR}" -B"${BERTRAND_BUILD_DIR}" -DCMAKE_INSTALL_PREFIX:PATH="${INSTALL_PATH}" -DBUILD_TESTING=off -DBERTRAND_BUILD_TESTING=on -DCMAKE_BUILD_TYPE=Debug -G Ninja > /dev/null
    cmake --build "${BERTRAND_BUILD_DIR}" --config Release
    CURRENT_DIR=$(pwd)
    cd "${BERTRAND_BUILD_DIR}"
    NUMBER_OF_TESTS=$(ctest -N -o | grep -E '^Total Tests: [0-9]+')
    cd "${CURRENT_DIR}"
    assertEquals "No tests found" "Total Tests: 0" "${NUMBER_OF_TESTS}"
}


testAllTestsEnabled()
{
    cmake "${ROOT_DIR}" -B"${BERTRAND_BUILD_DIR}" -DCMAKE_INSTALL_PREFIX:PATH="${INSTALL_PATH}" -DBUILD_TESTING=on -DBERTRAND_BUILD_TESTING=on -DCMAKE_BUILD_TYPE=Debug -G Ninja
    cmake --build "${BERTRAND_BUILD_DIR}" --config Release
    CURRENT_DIR=$(pwd)
    cd "${BERTRAND_BUILD_DIR}"
    NUMBER_OF_TESTS=$(ctest -N -o | grep -E '^Total Tests: [0-9]+')
    cd "${CURRENT_DIR}"
    assertNotEquals "Tests found" "Total Tests: 0" "${NUMBER_OF_TESTS}"
}

testUsageAsSubdirectory() {
    # install SI
    cmake "${ROOT_DIR}/test/installation-test-subdirectory" -B"${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release > /dev/null
    cmake --build "${BUILD_DIR}"
    assertEquals "build against installation successful" 0 $?
    cd "${BUILD_DIR}"
    NUMBER_OF_TESTS=$(ctest -N -o | grep -E '^Total Tests: [0-9]+')
    cd "${CURRENT_DIR}"
    assertEquals "Tests found" "Total Tests: 0" "${NUMBER_OF_TESTS}"
    
}

# Load shUnit2.
. shunit2