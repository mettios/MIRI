#include <math.h>
#include <float.h>
#include <cuda.h>

__global__ void gpu_Heat (float *h, float *g, int N, float* residuals) {
	// added residuals as an additional input for this function
	extern __shared__ float sdata[]; // shared memory for residuals
	int j = threadIdx.x + blockIdx.x * blockDim.x; // column
	int i = threadIdx.y + blockIdx.y * blockDim.y; // row
	int index = i * N + j; // index for jacobi computation
	unsigned int tid = threadIdx.x * blockDim.x + threadIdx.y; // unique id of the thread for this block
	// check corners
	if (i > 0 && i < (N-1) && j > 0 && j < (N-1)) {
		g[index] = 0.25 * (h[i * N + (j-1)] +     // left
				h[i * N + (j+1)] + 	  // right
				h[(i-1) * N + j] +  	  // top
				h[(i+1) * N + j]);  	  // bottom
	// Calculate diff value
		float diff = g[index] - h[index];
		sdata[tid] = diff * diff;
	}
	// wait for threads 
	__syncthreads();
	// apply reduction
	for (unsigned int s = blockDim.x * blockDim.y / 2; s > 32; s >>= 1) {
		if (tid > s) {
			sdata[tid] += sdata[tid + s];
		}
		__syncthreads();
	}
	// unroll last warp
	if (tid < 32) {
		sdata[tid] += sdata[tid + 32];
		sdata[tid] += sdata[tid + 16];
		sdata[tid] += sdata[tid + 8];
		sdata[tid] += sdata[tid + 4];
		sdata[tid] += sdata[tid + 2];
		sdata[tid] += sdata[tid + 1];
	}
	// Thread 0 holds the residual for this block
	if (tid == 0) {
		residuals[blockIdx.x * gridDim.y + blockIdx.y] = sdata[0];
	}
}
