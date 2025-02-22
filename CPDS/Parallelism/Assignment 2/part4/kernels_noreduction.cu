#include <math.h>
#include <float.h>
#include <cuda.h>

__global__ void gpu_Heat (float *h, float *g, int N) {

	//kernel computation
	int j = threadIdx.x + blockIdx.x * blockDim.x; // column
	int i = threadIdx.y + blockIdx.y * blockDim.y; // row
	int index = i * N + j;
	// check corners
	if (i > 0 && i < (N-1) && j > 0 && j < (N-1)) {
		g[index] = 0.25 * (h[i * N + (j-1)] + // left
				h[i * N + (j+1)] + 	  // right
				h[(i-1) * N + j] +  	  // top
				h[(i+1) * N + j]);  	  // bottom
	}
}
