#!/bin/bash

CMAKE_GENERATOR="Unix Makefiles"

SCRIPT_DIR="$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
[ -d ${BUILD_DIR} ] || { 
    mkdir -pv ${BUILD_DIR}
}

cmake -G "${CMAKE_GENERATOR}" -S ./ -B ${BUILD_DIR}
cmake --build ${BUILD_DIR} --config Release

echo "path to binary: ${BUILD_DIR}"
echo "done"
exit 0;
