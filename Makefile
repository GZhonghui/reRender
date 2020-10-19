cpu:
	$(info Building cpu lib.)

	$(info Building animation.)
	@g++ ./src/animation/animation.cpp -c -o ./build/animation.o --std=c++17 -fPIC
	@g++ ./src/animation/classic_physics.cpp -c -o ./build/classic_physics.o --std=c++17 -fPIC
	@g++ ./src/animation/pbf.cpp -c -o ./build/pbf.o --std=c++17 -fPIC
	@g++ ./src/animation/physics_object.cpp -c -o ./build/physics_object.o --std=c++17 -fPIC
	@g++ ./src/animation/surface.cpp -c -o ./build/surface.o --std=c++17 -fPIC

	$(info Building assets.)
	@g++ ./src/assets/model.cpp -c -o ./build/model.o --std=c++17 -fPIC
	@g++ ./src/assets/texture.cpp -c -o ./build/texture.o --std=c++17 -fPIC

	$(info Building basic.)
	@g++ ./src/basic/bbox.cpp -c -o ./build/bbox.o --std=c++17 -fPIC
	@g++ ./src/basic/bvh.cpp -c -o ./build/bvh.o --std=c++17 -fPIC
	@g++ ./src/basic/global.cpp -c -o ./build/global.o --std=c++17 -fPIC
	@g++ ./src/basic/intersection.cpp -c -o ./build/intersection.o --std=c++17 -fPIC
	@g++ ./src/basic/kdtree.cpp -c -o ./build/kdtree.o --std=c++17 -fPIC
	@g++ ./src/basic/material.cpp -c -o ./build/material.o --std=c++17 -fPIC
	@g++ ./src/basic/message.cpp -c -o ./build/message.o --std=c++17 -fPIC
	@g++ ./src/basic/object.cpp -c -o ./build/object.o --std=c++17 -fPIC
	@g++ ./src/basic/ray.cpp -c -o ./build/ray.o --std=c++17 -fPIC
	@g++ ./src/basic/skybox.cpp -c -o ./build/skybox.o --std=c++17 -fPIC

	$(info Building math.)
	@g++ ./src/math/basic_math.cpp -c -o ./build/basic_math.o --std=c++17 -fPIC
	@g++ ./src/math/matrix.cpp -c -o ./build/matrix.o --std=c++17 -fPIC
	@g++ ./src/math/noise.cpp -c -o ./build/noise.o --std=c++17 -fPIC
	@g++ ./src/math/tensor.cpp -c -o ./build/tensor.o --std=c++17 -fPIC
	@g++ ./src/math/vector.cpp -c -o ./build/vector.o --std=c++17 -fPIC

	$(info Building render.)
	@g++ ./src/render/bpath_tracing.cpp -c -o ./build/bpath_tracing.o --std=c++17 -fPIC
	@g++ ./src/render/direct_light.cpp -c -o ./build/direct_light.o --std=c++17 -fPIC
	@g++ ./src/render/mlight_transport.cpp -c -o ./build/mlight_transport.o --std=c++17 -fPIC
	@g++ ./src/render/path_tracing.cpp -c -o ./build/path_tracing.o --std=c++17 -fPIC
	@g++ ./src/render/photon_mapping.cpp -c -o ./build/photon_mapping.o --std=c++17 -fPIC
	@g++ ./src/render/render.cpp -c -o ./build/render.o --std=c++17 -fPIC
	@g++ ./src/render/render_algorithm.cpp -c -o ./build/render_algorithm.o --std=c++17 -fPIC

	$(info Building scene.)
	@g++ ./src/scene/scene.cpp -c -o ./build/scene.o --std=c++17 -fPIC

	$(info Building cpu api.)
	@g++ ./src/py_api.cpp -c -o ./build/py_api.o --std=c++17 -fPIC

	$(info Linking.)
	@g++ \
		./build/animation.o \
		./build/classic_physics.o \
		./build/pbf.o \
		./build/physics_object.o \
		./build/surface.o \
		./build/model.o \
		./build/texture.o \
		./build/bbox.o \
		./build/bvh.o \
		./build/global.o \
		./build/intersection.o \
		./build/kdtree.o \
		./build/material.o \
		./build/message.o \
		./build/object.o \
		./build/ray.o \
		./build/skybox.o \
		./build/basic_math.o \
		./build/matrix.o \
		./build/noise.o \
		./build/tensor.o \
		./build/vector.o \
		./build/bpath_tracing.o \
		./build/direct_light.o \
		./build/mlight_transport.o \
		./build/path_tracing.o \
		./build/photon_mapping.o \
		./build/render.o \
		./build/render_algorithm.o \
		./build/scene.o \
		./build/py_api.o \
		-l pthread \
		-shared \
		-o ./lib/render.so

gpu:
	$(info Building gpu lib.)

	$(info Building basic.)
	@g++ ./src/basic/message.cpp -c -o ./build/message.o --std=c++17 -fPIC

	$(info Building cuda math.)
	@nvcc ./cuda/cuda_math/cuda_basic.cu -c -o ./build/cuda_basic.o --std=c++17 -Xcompiler -fPIC

	$(info Building gpu api.)
	@nvcc ./cuda/cuda_global.cu -c -o ./build/cuda_global.o --std=c++17 -Xcompiler -fPIC

	$(info Linking.)
	@nvcc \
		./build/message.o \
		./build/cuda_basic.o \
		./build/cuda_global.o \
		-shared -o ./lib/render-gpu.so
