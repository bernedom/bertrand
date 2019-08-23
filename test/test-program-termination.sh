#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
ROOT_DIR=$(realpath ${SCRIPT_DIR}/../)
TEST_DIR=${SCRIPT_DIR}
INSTALL_PATH=$(realpath ~/SI-install)

setUp(){
    BUILD_DIR=$(mktemp -d)
}

tearDown(){
    if [ -d ${BUILD_DIR} ]; then
        rm -rf ${BUILD_DIR}
    fi
}

testForAbnormalProgrammTerminationInDebugMode(){
    cmake -DCMAKE_BUILD_TYPE=Debug ${ROOT_DIR} -B${BUILD_DIR} -G Ninja > /dev/null
    cmake --build ${BUILD_DIR} --target failing_contract
    assertEquals "build successful" 0 $?
    assertTrue "Test executable exists" "[ -f "${BUILD_DIR}/test/failing_contract" ]"
    
    $(${BUILD_DIR}/test/failing_contract)
    assertNotEquals "Executabled terminated non-zero" $? 0
}

testForNormalProgrammTerminationInReleaseMode(){
    cmake  -DCMAKE_BUILD_TYPE=Release ${ROOT_DIR} -B${BUILD_DIR} -G Ninja > /dev/null
    cmake --build ${BUILD_DIR} --target failing_contract
    assertEquals "build successful" 0 $?
    assertTrue "Test executable exists" "[ -f "${BUILD_DIR}/test/failing_contract" ]"
    
    $(${BUILD_DIR}/test/failing_contract)
    assertEquals "Executabled terminated normally"  0 $?
}


. shunit2