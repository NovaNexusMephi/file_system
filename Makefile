config:
	mkdir -p build
	cd build && cmake ..

build: config
	cd build && cmake --build . -j${nproc}

run: build
	@./build/src/dialog/FileSystemModel || [ $$? -eq 1 ]
clean:
	rm -rf build
