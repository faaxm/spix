#
# OS Specific dependencies
#
if [ "$TRAVIS_OS_NAME" = "osx" ]; then
    brew install qt5 clang-format;
fi

#
# Cross-Platform dependencies
#
mkdir deps && cd deps

git clone --recursive https://github.com/sgieseking/anyrpc.git
cd anyrpc && mkdir build && cd build
cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=OFF -DBUILD_WITH_LOG4CPLUS=OFF -DANYRPC_LIB_BUILD_SHARED=OFF ..
sudo cmake --build . --target install
cd ..

git clone --recursive https://github.com/google/googletest.git
cd googletest && mkdir build && cd build
cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
sudo cmake --build . --target install
cd ..

cd ..
