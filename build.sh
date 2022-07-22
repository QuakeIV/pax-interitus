
if command -v ninja &> /dev/null
then
    echo "building with ninja"
    cmake . -G Ninja -DCMAKE_BUILD_TYPE=Debug
    BUILD=ninja
else
    echo "could not find ninja, falling back to make"
    cmake . -DCMAKE_BUILD_TYPE=Debug
    BUILD=make
fi

${BUILD}
