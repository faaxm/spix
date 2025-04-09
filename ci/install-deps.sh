#!/bin/sh

# Exit on error
set -e

# Default: empty install prefix argument, use sudo for install
CMAKE_INSTALL_PREFIX_ARG=""
INSTALL_CMD="sudo cmake"

# Check for 'local_build' argument
if [ "$1" = "local_build" ]; then
  echo "Local build detected: Installing dependencies to ./deps-install and setting build type to Release."
  SCRIPT_PWD=$(pwd)
  INSTALL_DIR="$SCRIPT_PWD/deps-install"
  CMAKE_INSTALL_PREFIX_ARG="-DCMAKE_INSTALL_PREFIX=$INSTALL_DIR"
  INSTALL_CMD="cmake" # No sudo for local install
  # Override build type for local build
  export CI_BUILD_TYPE="Release"
else
  echo "Standard build detected: Installing dependencies to system default locations."
fi


# compile & install
mkdir -p deps && cd deps
# -> anyrpc
git clone --recursive https://github.com/sgieseking/anyrpc.git
cd anyrpc && mkdir -p build && cd build
if [ "$RUNNER_OS" = "Windows" ]; then
    cmake -G"Visual Studio 17 2022" ${CMAKE_INSTALL_PREFIX_ARG} -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_BUILD_TYPE=$CI_BUILD_TYPE -DBUILD_EXAMPLES=OFF -DBUILD_WITH_LOG4CPLUS=OFF -DANYRPC_LIB_BUILD_SHARED=${SHARED_LIBS} ..
    cmake --build . --target install --config $CI_BUILD_TYPE
else
    cmake -G"Unix Makefiles" ${CMAKE_INSTALL_PREFIX_ARG} -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_BUILD_TYPE=$CI_BUILD_TYPE -DBUILD_EXAMPLES=OFF -DBUILD_WITH_LOG4CPLUS=OFF -DANYRPC_LIB_BUILD_SHARED=${SHARED_LIBS} ..
    ${INSTALL_CMD} --build . --target install --config $CI_BUILD_TYPE
fi
# leave anyrpc
cd ../..
# -> googletest
git clone --recursive https://github.com/google/googletest.git
cd googletest && mkdir -p build && cd build
if [ "$RUNNER_OS" = "Windows" ]; then
    cmake -G"Visual Studio 17 2022" ${CMAKE_INSTALL_PREFIX_ARG} -Dgtest_force_shared_crt=ON -DCMAKE_BUILD_TYPE=$CI_BUILD_TYPE ..
    cmake --build . --target install --config $CI_BUILD_TYPE
else
    cmake -G"Unix Makefiles" ${CMAKE_INSTALL_PREFIX_ARG} -DCMAKE_BUILD_TYPE=$CI_BUILD_TYPE ..
    ${INSTALL_CMD} --build . --target install --config $CI_BUILD_TYPE
fi
cd ../..
# leave deps
cd ..
