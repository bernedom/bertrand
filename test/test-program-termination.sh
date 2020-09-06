#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
ROOT_DIR=$(realpath "${SCRIPT_DIR}/../")

setUp() {
    BUILD_DIR=$(mktemp -d)
}

tearDown() {
    if [ -d "${BUILD_DIR}" ]; then
        rm -rf "${BUILD_DIR}"
    fi
}

testForAbnormalProgramTerminationInDebugMode() {
    cmake -DCMAKE_BUILD_TYPE=Debug "${ROOT_DIR}" -B"${BUILD_DIR}" -DBERTRAND_BUILD_TESTING=on -G Ninja > /dev/null
    cmake --build "${BUILD_DIR}" --target failing_contract
    assertEquals "build successful" 0 $?
    assertTrue "Test executable exists" "[ -f ${BUILD_DIR}/test/bin/failing_contract ]"
    
    $("${BUILD_DIR}/test/bin/failing_contract")
    assertNotEquals "Executabled terminated non-zero" $? 0
}

testForNormalProgramTerminationInReleaseMode() {
    cmake -DCMAKE_BUILD_TYPE=Release "${ROOT_DIR}" -B"${BUILD_DIR}" -DBERTRAND_BUILD_TESTING=on -G Ninja > /dev/null
    cmake --build "${BUILD_DIR}" --target failing_contract
    assertEquals "build successful" 0 $?
    assertTrue "Test executable exists" "[ -f ${BUILD_DIR}/test/bin/failing_contract ]"
    
    $("${BUILD_DIR}/test/bin/failing_contract")
    assertEquals "Executabled terminated normally" 0 $?
}

testForMessageOnProgramTerminationInDebugMode() {
    cmake -DCMAKE_BUILD_TYPE=Debug "${ROOT_DIR}" -B"${BUILD_DIR}" -DBERTRAND_BUILD_TESTING=on -G Ninja >/dev/null
    cmake --build "${BUILD_DIR}" --target failing_contract
    assertEquals "build successful" 0 $?
    assertTrue "Test executable exists" "[ -f ${BUILD_DIR}/test/bin/failing_contract ]"
    
    OUTPUT=$("${BUILD_DIR}/test/bin/failing_contract" 2>&1)
    assertNotEquals "Executabled terminated non-zero" $? 0
    TEXT=$OUTPUT
    EXPECTED_TEXT="Cannot be false"
    if [[ $OUTPUT =~ "${EXPECTED_TEXT}" ]]; then
        TEXT=$EXPECTED_TEXT
    fi
    assertEquals "Text is there" "${EXPECTED_TEXT}" "${TEXT}"
}

testForMessageFileAndLinenumberOnProgramTerminationInDebugMode() {
    cmake -DCMAKE_BUILD_TYPE=Debug "${ROOT_DIR}" -B"${BUILD_DIR}" -DBERTRAND_BUILD_TESTING=on -G Ninja >/dev/null
    cmake --build "${BUILD_DIR}" --target failing_contract
    assertEquals "build successful" 0 $?
    assertTrue "Test executable exists" "[ -f ${BUILD_DIR}/test/bin/failing_contract ]"
    
    OUTPUT=$("${BUILD_DIR}/test/bin/failing_contract" 2>&1)
    assertNotEquals "Executabled terminated non-zero" $? 0
    TEXT=$OUTPUT
    EXPECTED_TEXT="Assert at: ${SCRIPT_DIR}/src/failing_contract.cc:4: Require ('false') Cannot be false"
    if [[ $OUTPUT =~ "${EXPECTED_TEXT}" ]]; then
        TEXT=$EXPECTED_TEXT
    fi
    assertEquals "Text is there" "${EXPECTED_TEXT}" "${TEXT}"
}

testForAbnormalProgramTerminationInReleaseModeWhenContractsForceEnabled() {
    
    cmake -DCMAKE_BUILD_TYPE=Release "${ROOT_DIR}" -B"${BUILD_DIR}" -DCMAKE_CXX_FLAGS="-DBERTRAND_ENABLE_CONTRACTS" -DBERTRAND_BUILD_TESTING=on -G Ninja >/dev/null
    cmake --build "${BUILD_DIR}" --target failing_contract
    assertEquals "build successful" 0 $?
    assertTrue "Test executable exists" "[ -f ${BUILD_DIR}/test/bin/failing_contract ]"
    
    $("${BUILD_DIR}/test/bin/failing_contract")
    assertNotEquals "Executabled terminated non-zero" $? 0
}


testForNormalProgramTerminationInDebugModeWhenContractsForceDisabled() {
    
    cmake -DCMAKE_BUILD_TYPE=Debug "${ROOT_DIR}" -B"${BUILD_DIR}" -DCMAKE_CXX_FLAGS="-DBERTRAND_DISABLE_CONTRACTS" -DBERTRAND_BUILD_TESTING=on -G Ninja >/dev/null
    cmake --build "${BUILD_DIR}" --target failing_contract
    assertEquals "build successful" 0 $?
    assertTrue "Test executable exists" "[ -f ${BUILD_DIR}/test/bin/failing_contract ]"
    
    $("${BUILD_DIR}/test/bin/failing_contract")
    assertEquals "Executabled terminated normally" 0 $?
}

testForCompilationFailureWhenContractsForceEnabledAndForceDisabledTogether() {
    
    cmake -DCMAKE_BUILD_TYPE=Debug "${ROOT_DIR}" -B"${BUILD_DIR}" -DCMAKE_CXX_FLAGS="-DBERTRAND_DISABLE_CONTRACTS -DBERTRAND_ENABLE_CONTRACTS" -DBERTRAND_BUILD_TESTING=on -G Ninja >/dev/null
    cmake --build "${BUILD_DIR}" --target failing_contract
    assertNotEquals "build fails" 0 $?
    
}

. shunit2
