#!/bin/bash

SCRIPT_DIR="$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
[ -d ${BUILD_DIR} ] || { 
    mkdir -pv ${BUILD_DIR}
}

if [[ "$(uname -s)" == *"MINGW"* ]]; then
	cmake -G "Visual Studio 16 2019" -T v142 -S ./ -B ${BUILD_DIR}
else
	cmake -G "Unix Makefiles" -S ./ -B ${BUILD_DIR}
fi

echo "done"
exit 0;
