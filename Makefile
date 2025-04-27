config:
	mkdir build
	cd build && cmake ..

build: config
	cd build && cmake --build . -j${nproc}

quick_build:
	cd build && cmake --build . -j${nproc}

run: build
	./build/bin/filesystem

clean:
	rm -rf build
