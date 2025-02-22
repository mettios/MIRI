/*
 * Iterative solver for heat distribution
 */

#include <stdio.h>
#include <stdlib.h>
#include "heat.h"

void usage(char *s) {
    fprintf(stderr, "Usage: %s <input file> [result file]\n\n", s);
}

int main(int argc, char *argv[]) {
    unsigned iter;
    FILE *infile, *resfile;
    char *resfilename;

    // Algorithmic parameters
    algoparam_t param;
    int np;
    double runtime, flop;
    double residual = 0.0;

    // Check command-line arguments
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    // Open input file
    if (!(infile = fopen(argv[1], "r"))) {
        fprintf(stderr, "\nError: Cannot open \"%s\" for reading.\n\n", argv[1]);
        usage(argv[0]);
        return 1;
    }

    // Determine result file name and open it
    resfilename = (argc >= 3) ? argv[2] : "heat.ppm";
    if (!(resfile = fopen(resfilename, "w"))) {
        fprintf(stderr, "\nError: Cannot open \"%s\" for writing.\n\n", resfilename);
        usage(argv[0]);
        return 1;
    }

    // Parse input file
    if (!read_input(infile, &param)) {
        fprintf(stderr, "\nError: Error parsing input file.\n\n");
        usage(argv[0]);
        return 1;
    }
    print_params(&param);

    // Initialize solver
    if (!initialize(&param)) {
        fprintf(stderr, "Error in Solver initialization.\n\n");
        usage(argv[0]);
        return 1;
    }

    // Compute full grid size (including boundary points)
    np = param.resolution + 2;

    // Start timer
    runtime = wtime();

    iter = 0;
    while (1) {
        switch (param.algorithm) {
            case 0: // JACOBI METHOD
                residual = relax_jacobi(param.u, param.uhelp, np, np);
                // Copy updated values from uhelp to u
                for (int i = 0; i < np; i++)
                    for (int j = 0; j < np; j++)
                        param.u[i * np + j] = param.uhelp[i * np + j];
                break;
            case 1: // RED-BLACK METHOD
                residual = relax_redblack(param.u, np, np);
                break;
            case 2: // GAUSS-SEIDEL METHOD
                residual = relax_gauss(param.u, np, np);
                break;
        }

        iter++;

        // Check for convergence
        if (residual < 0.00005)
            break;

        // Check iteration limit (if applicable)
        if (param.maxiter > 0 && iter >= param.maxiter)
            break;
    }

    // Compute floating point operations
    flop = iter * 11.0 * param.resolution * param.resolution;

    // Stop timer
    runtime = wtime() - runtime;

    // Output results
    fprintf(stdout, "Time: %04.3f s ", runtime);
    fprintf(stdout, "(%3.3f GFlop => %6.2f MFlop/s)\n", flop / 1e9, flop / runtime / 1e6);
    fprintf(stdout, "Convergence to residual=%f: %d iterations\n", residual, iter);

    // Prepare visualization data
    coarsen(param.u, np, np, param.uvis, param.visres + 2, param.visres + 2);
    write_image(resfile, param.uvis, param.visres + 2, param.visres + 2);

    // Clean up resources
    finalize(&param);

    return 0;
}
