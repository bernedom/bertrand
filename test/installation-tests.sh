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
    
    assertEquals "version in cmake (${CMAKE_VERSION}) does not match conan version (${CONAN_VERSION})" $CMAKE_VERSION $CONAN_VERSION
}

testPureCmakeInstallation(){
    # install SI
    cmake ${ROOT_DIR} -B${BERTRAND_BUILD_DIR} -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_PATH} -DBUILD_TESTING=off -G Ninja
    cmake --build ${BERTRAND_BUILD_DIR} --config Release --target install
    assertEquals "Installation build successful" 0 $?
    cmake ${ROOT_DIR}/test/installation-test -B${BUILD_DIR} -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_PATH} -G Ninja
    cmake --build ${BUILD_DIR}
    assertEquals "build against installation successful" 0 $?
    
}


# Load shUnit2.
. shunit2