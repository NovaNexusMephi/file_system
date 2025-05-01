config:
	mkdir -p build
	cd build && cmake ..

build: config
	cd build && cmake --build . -j${nproc}

run: build
	./build/bin/filesystem

clean:
	rm -rf build
