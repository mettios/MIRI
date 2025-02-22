#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "mpi.h"

/* This example handles a 12 x 12 mesh processed on P=4 processes. */
/* NOTE: The program WILL ABORT if NOT RUN WITH EXACTLY P PROCESSES. */

#define maxn 12
#define P 4

int main(argc, argv)
int argc;
char **argv;
{
    int rank, value, size, errcnt, toterr, i, j, itcnt;
    int i_first, i_last;
    int next_nbr, prev_nbr;
    MPI_Status status;
    double diffnorm, gdiffnorm;
    double xlocal[(maxn / P) + 2][maxn];
    double xnew[(maxn / P) + 1][maxn]; /* Auxiliary matrix used in the Jacobi method. */
    double x[maxn][maxn];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != P) {
        printf("This program only works when run with %d processes. Aborting.\n", P);
        usleep(10);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    i_first = 1;
    i_last = maxn / size;

    if (rank == 0)        i_first++;
    if (rank == size - 1) i_last--;

    for (i = 1; i <= maxn / size; i++) 
        for (j = 0; j < maxn; j++) 
            xlocal[i][j] = rank;
    
    for (j = 0; j < maxn; j++) {
        xlocal[i_first - 1][j] = -1;
        xlocal[i_last + 1][j] = -1;
    }

    next_nbr = rank + 1;
    if (next_nbr >= size) next_nbr = MPI_PROC_NULL;
    prev_nbr = rank - 1;
    if (prev_nbr < 0) prev_nbr = MPI_PROC_NULL;

    itcnt = 0;
    do {
        MPI_Send(xlocal[maxn / size], maxn, MPI_DOUBLE, next_nbr, 0, MPI_COMM_WORLD);
        MPI_Recv(xlocal[0], maxn, MPI_DOUBLE, prev_nbr, 0, MPI_COMM_WORLD, &status);
        MPI_Send(xlocal[1], maxn, MPI_DOUBLE, prev_nbr, 1, MPI_COMM_WORLD);
        MPI_Recv(xlocal[maxn / size + 1], maxn, MPI_DOUBLE, next_nbr, 1, MPI_COMM_WORLD, &status);

        itcnt++;
        diffnorm = 0.0;
        for (i = i_first; i <= i_last; i++) 
            for (j = 1; j < maxn - 1; j++) {
                xnew[i][j] = (xlocal[i][j + 1] + xlocal[i][j - 1] +
                              xlocal[i + 1][j] + xlocal[i - 1][j]) / 4.0;
                diffnorm += (xnew[i][j] - xlocal[i][j]) * (xnew[i][j] - xlocal[i][j]);
            }

        for (i = i_first; i <= i_last; i++) 
            for (j = 1; j < maxn - 1; j++) 
                xlocal[i][j] = xnew[i][j];

        MPI_Allreduce(&diffnorm, &gdiffnorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        gdiffnorm = sqrt(gdiffnorm);

        if (rank == 0) 
            printf("At iteration %d, diff is %e\n", itcnt, gdiffnorm);
    } while (gdiffnorm > 1.0e-2 && itcnt < 100);

    MPI_Gather(xlocal[1], maxn * (maxn / size), MPI_DOUBLE, x, maxn * (maxn / size), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Final solution is\n");
        for (i = 0; i < maxn; i++) {
            for (j = 0; j < maxn; j++) 
                printf("%f ", x[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
