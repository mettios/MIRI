# CPDS/Parallelism Laboratory Assignment 1: Parallel Programming with MPI - A Distributed Data Structure

## Overview

This assignment involves learning parallel programming with MPI (Message Passing Interface) by implementing a simple distributed data structure and solving a numerical problem using the Jacobi iterative method. The goal is to distribute data across multiple processes while handling communication between processes using MPI primitives. The assignment consists of two main tasks:

1. **Distributed Data Structure**: Implementing and testing a parallelized data structure that is divided among multiple MPI processes.
2. **Jacobi Iterative Method**: Solving a numerical problem (Laplace equation) using the Jacobi method, with parallelism and communication between processes.

## Tasks

### 1. Distributed Data Structure

In this part, we implement a distributed two-dimensional array (matrix) that is split across multiple processes. Each process holds a segment (slab) of the matrix and exchanges data with neighboring processes using ghost points.

#### Key Concepts
- **Ghost Points**: Additional rows are allocated in each process's local array to store boundary data received from neighboring processes.
- **Communication**: Processes communicate using MPI Send, Recv, Isend, and Irecv to exchange boundary data.
- **Linear Topology**: Processes are arranged in a linear array where each process communicates with its neighboring processes.

#### Steps:
1. **Data Structure**: The main data structure is a two-dimensional matrix, `x[maxn][maxn]`. Each process holds a portion of this matrix in a local array `xlocal[maxn/size + 2][maxn]`, where additional rows are allocated for ghost points.
2. **MPI Communication**: Implement communication to send and receive data between neighboring processes:
   - `MPI_Send` and `MPI_Recv` (blocking communication)
   - `MPI_Isend` and `MPI_Irecv` (non-blocking communication)
   - `MPI_Sendrecv` (combined send and receive)
3. **Validation**: After communication, check that the segment data remains consistent and that ghost points are correctly updated.

### 2. Jacobi Iterative Method

In this part, we implement the Jacobi iterative method for solving the Laplace equation using parallelism. The method involves iteratively updating matrix values based on neighboring points, using the average of the neighboring points to approximate the solution.

#### Key Concepts
- **Jacobi Iteration**: The core of the method involves updating each point based on its four neighbors.
- **Convergence**: The iteration continues until the difference between the new and old values (`diffnorm`) is below a certain threshold or a maximum number of iterations is reached.

#### Steps:
1. **Initialization**: Initialize the matrix with boundary values (-1 for the halo and the process rank for interior points).
2. **Jacobi Iteration**: Perform the iteration by updating interior points based on the average of their neighbors.
3. **Convergence Testing**: Use MPI Allreduce to compute the global `diffnorm` and check for convergence.
4. **Output**: After convergence, the solution is collected by process 0 and outputted.

## Deliverables
1. **Code Completion**: Complete the provided code files by filling in missing MPI primitives and handling communication between processes.
   - `par_data_struct.c` (blocking communication)
   - `par_data_struct_nonblocking.c` (non-blocking communication)
   - `par_data_struct_sendreceive.c` (combined send and receive)
   - `Jacobi.c` (Jacobi method with parallel communication)
2. **Error Reporting**: Identify and fix errors in the code, and document the changes made to fix them.

## Execution Instructions

1. **Compilation**: Use `make` to compile the code, which uses `mpicc` for MPI compilation.
2. **Execution**: Run the code on a local machine or in a batch system (Boada) using the following commands:
   - Local: `mpirun -np 4 MPI_EXECUTABLE_NAME_HERE`
   - Boada: `sbatch submit-mpi.sh MPI_EXECUTABLE_NAME_HERE`
3. **Output**: The output for each program will be written to a dynamically generated file: `Output-$PROGRAM-$MPI_PROCESSES.txt`.

## Additional Notes
- The code is divided into three different communication schemes (blocking, non-blocking, and send/receive) to demonstrate different MPI primitives.
- The final solution is collected and printed by process 0 after the Jacobi method converges.

## Files
- `par_data_struct.c`: Implements blocking communication with MPI Send/Recv.
- `par_data_struct_nonblocking.c`: Uses non-blocking MPI communication (MPI Isend/Irecv).
- `par_data_struct_sendreceive.c`: Combines send and receive using MPI Sendrecv.
- `Jacobi.c`: Implements the Jacobi iterative method with parallel communication.
