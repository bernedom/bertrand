#!/bin/bash
# file: test/installation-tests.sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
ROOT_DIR=$(realpath ${SCRIPT_DIR}/../)

setUp(){
    BERTRAND_BUILD_DIR=$(mktemp -d)
    INSTALL_PATH=$(mktemp -d)
    BUILD_DIR=$(mktemp -d)
}

tearDown(){
    if [ -d ${BERTRAND_BUILD_DIR} ]; then
        rm -rf ${BERTRAND_BUILD_DIR}
    fi
    
    if [ -d ${INSTALL_PATH} ]; then
        rm -rf ${INSTALL_PATH}
    fi
    
    if [ -d ${BUILD_DIR} ]; then
        rm -rf ${BUILD_DIR}
    fi
}

testVersionNumberConsistency()
{
    ORIG_DIR=$(pwd)
    cmake ${ROOT_DIR} -B${BERTRAND_BUILD_DIR} -DBUILD_TESTING=off -G Ninja
    cd ${BERTRAND_BUILD_DIR}
    CMAKE_VERSION=$(cmake --system-information|grep -E "VERSION:STATIC"|grep -E -o '[0-9]+\.[0-9]+\.[0-9]+')
    cd ${ROOT_DIR};
    CONAN_VERSION=$(python3 -c 'from conanfile import bertrandConan; print(bertrandConan.version)')
    cd ${ORIG_DIR}
    GIT_VERSION_EXACT=$(git describe --tags | grep -E -o '^[0-9]+\.[0-9]+\.[0-9]+$')
    
    assertEquals "version in cmake (${CMAKE_VERSION}) does not match conan version (${CONAN_VERSION})" $CMAKE_VERSION $CONAN_VERSION
    
    if [ "${GIT_VERSION_EXACT}" != "" ]; then
        assertEquals "version in files (${CMAKE_VERSION}) does not match git tag for release  (${GIT_VERSION_EXACT})" ${CMAKE_VERSION} ${GIT_VERSION_EXACT}
    else
        GIT_VERSION=$(git describe --tags | grep -E -o '^[0-9]+\.[0-9]+\.[0-9]+')
        assertNotEquals "version in files (${CMAKE_VERSION}) matches already existing release (${GIT_VERSION}):" ${CMAKE_VERSION} ${GIT_VERSION}
    fi
}

testPureCmakeInstallation(){
    # install bertrand
    cmake ${ROOT_DIR} -B${BERTRAND_BUILD_DIR} -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_PATH} -DBUILD_TESTING=off -G Ninja
    cmake --build ${BERTRAND_BUILD_DIR} --config Release --target install
    assertEquals "Installation build successful" 0 $?
    cmake ${ROOT_DIR}/test/installation-test -B${BUILD_DIR} -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_PATH} -G Ninja
    cmake --build ${BUILD_DIR}
    assertEquals "build against installation successful" 0 $?
    
}

testCpackInstallation(){
    # install bertrand
    cmake ${ROOT_DIR} -B${BERTRAND_BUILD_DIR} -DCPACK_PACKAGE_FILE_NAME=install-bertrand -DBUILD_TESTING=off -G Ninja
    cmake --build ${BERTRAND_BUILD_DIR} --config Release --target package
    assertEquals "Installation build successful" 0 $?
    ${BERTRAND_BUILD_DIR}/install-bertrand.sh --prefix=${INSTALL_PATH} --skip-license --exclude-subdir
    assertEquals "Installation script successful" 0 $?
    
    cmake ${ROOT_DIR}/test/installation-test -B${BUILD_DIR} -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_PATH} -G Ninja
    cmake --build ${BUILD_DIR}
    assertEquals "build against installation successful" 0 $?
    
}

testConanInstallation()
{
    
    # preliminary cleanup
    conan remove -f *@bertrand/testing
    
    conan create ${ROOT_DIR} bertrand/testing
    assertEquals "Conan installation build successful" 0 $?
    conan install -if ${BUILD_DIR} ${ROOT_DIR}/test/conan-installation-test
    assertEquals "Conan installation successful" 0 $?
    
    cmake ${ROOT_DIR}/test/conan-installation-test -B${BUILD_DIR} -G Ninja
    cmake --build ${BUILD_DIR}
    assertEquals "build against installation successful" 0 $?
    
    # cleanup
    conan remove -f *@bertrand/testing
    
    
}


# Load shUnit2.
. shunit2