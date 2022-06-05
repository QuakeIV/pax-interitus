
if command -v ninja &> /dev/null
then
    echo "building with ninja"
    cmake . -G Ninja
    BUILD=ninja
else
    echo "could not find ninja, falling back to make"
    cmake .
    BUILD=make
fi

${BUILD}
