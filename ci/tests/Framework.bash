#!/bin/bash
####
# Framework.bash:
#
# Run the tests on the software through fp-util.
####
 CTEST_OUTPUT_ON_FAILURE=1

 SCRIPT_DIR="$(dirname ${BASH_SOURCE})"
. "${SCRIPT_DIR}/../helpers.bash"
. ${SCRIPT_DIR}/fputil.bash

#### NEEDED ENVIRONMENT ####
 FPRIME_DIR="$(cd ${SCRIPT_DIR}/../..; pwd)"
 LOG_DIR="${FPRIME_DIR}/ci-Framework-logs-$(date +"%Y-%m-%dT%H%M%S")"
mkdir -p "${LOG_DIR}"

# Directory to be used for Framework CI test
 FPUTIL_DEPLOYS="${FPRIME_DIR}"

echo -e "${BLUE}Starting CI test ${FPUTIL_DEPLOYS} Ref${NOCOLOR}"


 CMAKE_EXTRA_SETTINGS=""
echo -e "${BLUE}Testing ${FPUTIL_DEPLOYS} against fprime-util targets: ${FPUTIL_TARGETS[@]}${NOCOLOR}"
export CHECK_TARGET_PLATFORM="native"
 target  "${FPUTIL_TARGETS[@]}"

     [[ "${target}" == "generate" ]]
    
        rm -rf "${FPUTIL_DEPLOYS}/build-fprime-automatic-"*
   
    fputil_action "${FPUTIL_DEPLOYS}" "${target}"


# Test Completed
echo -e "${GREEN}CI test ${FPUTIL_DEPLOYS} Framework SUCCESSFUL${NOCOLOR}"

archive_logs
