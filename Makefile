
.PHONY: pax
pax:
	cmake -G Ninja -B build/ . -DCMAKE_BUILD_TYPE=Debug
	cd build/; ninja
