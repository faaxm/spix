mkdir build && cd build

if [ "$TRAVIS_OS_NAME" = "osx" ]; then
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DSPIX_BUILD_TESTS=ON -DSPIX_BUILD_EXAMPLES=ON -DCMAKE_PREFIX_PATH="$(brew --prefix qt)" ..
else
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DSPIX_BUILD_TESTS=ON -DSPIX_BUILD_EXAMPLES=ON ..
fi

cat compile_commands.json
cmake  --build .
ctest -VV .
