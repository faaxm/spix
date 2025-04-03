#!/bin/sh

# Exit on error
set -e

# compile & install
mkdir deps && cd deps
# -> anyrpc
git clone --recursive https://github.com/sgieseking/anyrpc.git
cd anyrpc && mkdir build && cd build
if [[ $RUNNER_OS == "Windows" ]]; then
    cmake -G"Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=$CI_BUILD_TYPE -DBUILD_EXAMPLES=OFF -DBUILD_WITH_LOG4CPLUS=OFF -DANYRPC_LIB_BUILD_SHARED=${SHARED_LIBS} ..
    cmake --build . --target install --config $CI_BUILD_TYPE
else
    cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=$CI_BUILD_TYPE -DBUILD_EXAMPLES=OFF -DBUILD_WITH_LOG4CPLUS=OFF -DANYRPC_LIB_BUILD_SHARED=${SHARED_LIBS} ..
    sudo cmake --build . --target install --config $CI_BUILD_TYPE
fi
# leave anyrpc
cd ../..
# -> googletest
git clone --recursive https://github.com/google/googletest.git
cd googletest && mkdir build && cd build
if [[ $RUNNER_OS == "Windows" ]]; then
    cmake -G"Visual Studio 17 2022" -Dgtest_force_shared_crt=ON -DCMAKE_BUILD_TYPE=$CI_BUILD_TYPE ..
    cmake --build . --target install --config $CI_BUILD_TYPE
else
    cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=$CI_BUILD_TYPE ..
    sudo cmake --build . --target install --config $CI_BUILD_TYPE
fi
cd ../..
# leave deps
cd ..
