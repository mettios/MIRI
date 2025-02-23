# Heat Equation Parallelization Assignment (CPDS Lab 2)

This repository contains the solution to the *Heat Equation Parallelization* assignment, focusing on the application of parallel programming models: **OpenMP**, **MPI**, and **CUDA**. The goal is to parallelize a sequential code that solves the heat equation using the **Jacobi** and **Gauss-Seidel** solvers, each offering different parallelization characteristics.

## Overview

The code simulates the diffusion of heat in a solid body and uses solvers like Jacobi and Gauss-Seidel, with a focus on parallelizing the program across multiple threads, processes, and GPUs. The implementation includes:

- **Sequential Code**: Heat equation solved using Jacobi and Gauss-Seidel solvers.
- **OpenMP Parallelization**: Shared-memory parallelization for multi-core CPUs.
- **MPI Parallelization**: Distributed-memory parallelization for multi-node clusters.
- **CUDA Parallelization**: GPU-based parallelization to accelerate computations.

## Key Concepts

1. **Sequential Heat Diffusion Program**:
   - Simulates heat diffusion using solvers.
   - Generates images representing temperature distribution after simulation.

2. **OpenMP Parallelization**:
   - Implements shared-memory parallelization using OpenMP.
   - Uses geometric block decomposition to divide the matrix into blocks and parallelizes the workload across threads.
   - Includes parallelization of Jacobi and Gauss-Seidel solvers using OpenMP pragmas (`#pragma`).

3. **MPI Parallelization**:
   - Implements message-passing parallelization.
   - Divides the matrix and assigns computation tasks to multiple processes.
   - Includes proper communication for boundary exchange between processes and ensures global residual control.

4. **CUDA Parallelization**:
   - Implements GPU-based parallelization using CUDA.
   - Each thread computes a single element in the matrix.
   - Focus on optimizing residual calculation on the GPU using fast reduction techniques and shared memory.

## Files and Structure

### Part 1 - Sequential Code
- `heat.c`: Sequential heat diffusion solver using Jacobi and Gauss-Seidel.
- `test.dat`: Configuration file to specify simulation parameters.
- `heat.ppm`: Output image representing temperature distribution.

### Part 2 - OpenMP Parallelization
- `heat-omp.c`: Parallelized Jacobi solver using OpenMP.
- `solver-omp.c`: Parallelization logic for the solver.
- `submit-omp.sh`: SLURM script for executing OpenMP version on a cluster.
- `time-heat-omp.txt`: Performance measurement for OpenMP execution.

### Part 3 - MPI Parallelization
- `heat-mpi.c`: MPI-based parallelization for Jacobi and Gauss-Seidel solvers.
- `submit-mpi.sh`: SLURM script for MPI-based execution.
- `mpi_output.txt`: Performance and error logs for MPI runs.

### Part 4 - CUDA Parallelization
- `heat-CUDA.cu`: CUDA-based Jacobi solver.
- `kernels.cu`: GPU kernel for heat diffusion computation.
- `submit-cuda.sh`: SLURM script for CUDA execution.
- `cuda_output.txt`: GPU performance logs.

## Deliverables

1. **Report (PDF)**: Explains the parallelization techniques used for each model (OpenMP, MPI, CUDA) and the speedups achieved.
2. **Source Code**: Includes the C source codes for OpenMP, MPI, and CUDA parallelizations.

## How to Run the Code

### OpenMP
1. Compile: `make heat-omp`
2. Execute (with 4 threads): `OMP_NUM_THREADS=4 ./heat-omp test.dat`
3. Submit to SLURM: `sbatch submit-omp.sh 8`

### MPI
1. Compile: `make heat-mpi`
2. Execute (with 4 MPI processes): `sbatch submit-mpi.sh`
3. Modify and tune parallelization for Gauss-Seidel solver.

### CUDA
1. Compile: `make heat-CUDA`
2. Execute with CUDA: `sbatch submit-cuda.sh`
3. Test performance and compare with CPU version.

## Performance Metrics

- **Execution Time**: Time taken for different parallel models (OpenMP, MPI, CUDA) for various input sizes.
- **Speedup**: Comparison of execution time with respect to the serial execution.

---

This project provides a deep dive into parallel programming techniques, demonstrating the use of OpenMP, MPI, and CUDA to solve computational problems in high-performance computing.
