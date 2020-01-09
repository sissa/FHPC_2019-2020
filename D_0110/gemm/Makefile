
all: gemm.x gemm_gpu.x

cpu: gemm.x

gpu: gemm_gpu.x

gemm.x: gemm.c
	gcc -DUSE_FLOAT $^ -lopenblas -o $@

gemm_gpu.x: gemm_gpu.c
	nvcc -DUSE_FLOAT $^ -lopenblas -lcublas  -o $@

clean:
	rm -rf *.x
