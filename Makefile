all:
	g++ ./src/bvh.cpp -c -o ./build/bvh.o --std=c++11 -fPIC
	g++ ./src/fluid.cpp -c -o ./build/fluid.o -l pthread --std=c++11 -fPIC
	g++ ./src/global.cpp -c -o ./build/global.o --std=c++11 -fPIC
	g++ ./src/material.cpp -c -o ./build/material.o --std=c++11 -fPIC
	g++ ./src/math.cpp -c -o ./build/math.o --std=c++11 -fPIC
	g++ ./src/message.cpp -c -o ./build/message.o --std=c++11 -fPIC
	g++ ./src/object.cpp -c -o ./build/object.o --std=c++11 -fPIC
	g++ ./src/ray.cpp -c -o ./build/ray.o --std=c++11 -fPIC
	g++ ./src/render.cpp -c -o ./build/render.o -l pthread --std=c++11 -fPIC
	g++ ./src/surface.cpp -c -o ./build/surface.o --std=c++11 -fPIC
	g++ ./build/*.o -fPIC -shared -o ./lib/fluid.so