#include "heat.h"
#include <omp.h>

#define NB 8

#define min(a,b) ( ((a) < (b)) ? (a) : (b) )
#define max(a,b) ( ((a) > (b)) ? (a) : (b) )
/*
 * Blocked Jacobi solver: one iteration step
 */
double relax_jacobi (double *u, double *utmp, unsigned sizex, unsigned sizey)
{
    double diff, sum=0.0;
    int nbx, bx, nby, by;
  
    nbx = NB;
    bx = sizex/nbx;
    nby = NB;
    by = sizey/nby;
    //#pragma omp parallel for collapse(2) private(diff) reduction(+:sum) shared(u, utmp)
    /*for (int i = 1; i < sizex - 1; i++)
        for (int j = 1; j < sizey - 1; j++) {
            utmp[i * sizey + j] = 0.25 * (u[i * sizey + (j - 1)] +  // left
                                          u[i * sizey + (j + 1)] +  // right
                                          u[(i - 1) * sizey + j] +  // top
                                          u[(i + 1) * sizey + j]);  // bottom
            diff = utmp[i * sizey + j] - u[i * sizey + j];
            sum += diff * diff;
        }
    */
//    #pragma omp parallel for collapse(2) private(diff) reduction(+:sum)
    #pragma omp parallel for collapse(2) private(diff) reduction(+:sum) shared(u, utmp)
    for (int ii=0; ii<nbx; ii++)
        for (int jj=0; jj<nby; jj++) 
            for (int i=1+ii*bx; i<=min((ii+1)*bx, sizex-2); i++) 
                for (int j=1+jj*by; j<=min((jj+1)*by, sizey-2); j++) {
	            utmp[i*sizey+j]= 0.25 * (u[ i*sizey     + (j-1) ]+  // left
					     u[ i*sizey     + (j+1) ]+  // right
				             u[ (i-1)*sizey + j     ]+  // top
				             u[ (i+1)*sizey + j     ]); // bottom
	            diff = utmp[i*sizey+j] - u[i*sizey + j];
	            sum += diff * diff; 
	        }
     
    return sum;
}

/*
 * Blocked Red-Black solver: one iteration step
 */
double relax_redblack (double *u, unsigned sizex, unsigned sizey)
{
    double unew, diff, sum=0.0;
    int nbx, bx, nby, by;
    int lsw;

    nbx = NB;
    bx = sizex/nbx;
    nby = NB;
    by = sizey/nby;
    // Computing "Red" blocks
    for (int ii=0; ii<nbx; ii++) {
        lsw = ii%2;
        for (int jj=lsw; jj<nby; jj=jj+2) 
            for (int i=1+ii*bx; i<=min((ii+1)*bx, sizex-2); i++) 
                for (int j=1+jj*by; j<=min((jj+1)*by, sizey-2); j++) {
	            unew= 0.25 * (    u[ i*sizey	+ (j-1) ]+  // left
				      u[ i*sizey	+ (j+1) ]+  // right
				      u[ (i-1)*sizey	+ j     ]+  // top
				      u[ (i+1)*sizey	+ j     ]); // bottom
	            diff = unew - u[i*sizey+ j];
	            sum += diff * diff; 
	            u[i*sizey+j]=unew;
	        }
    }

    // Computing "Black" blocks
    for (int ii=0; ii<nbx; ii++) {
        lsw = (ii+1)%2;
        for (int jj=lsw; jj<nby; jj=jj+2) 
            for (int i=1+ii*bx; i<=min((ii+1)*bx, sizex-2); i++) 
                for (int j=1+jj*by; j<=min((jj+1)*by, sizey-2); j++) {
	            unew= 0.25 * (    u[ i*sizey	+ (j-1) ]+  // left
				      u[ i*sizey	+ (j+1) ]+  // right
				      u[ (i-1)*sizey	+ j     ]+  // top
				      u[ (i+1)*sizey	+ j     ]); // bottom
	            diff = unew - u[i*sizey+ j];
	            sum += diff * diff; 
	            u[i*sizey+j]=unew;
	        }
    }

    return sum;
}

/*
 * Blocked Gauss-Seidel solver: one iteration step
 */
double relax_gauss_explicit (double *u, unsigned sizex, unsigned sizey)
{
    double unew, diff, sum=0.0;
    int nbx, bx, nby, by;

    nbx = NB;
//    bx = sizex/nbx;
    bx = sizex / nbx + (sizex % nbx != 0);
    nby = NB;
//    by = sizey/nby;
    by = sizey / nby + (sizey % nby != 0);
    
    int aux_depend[nbx][nby];

    #pragma omp parallel
    {
	#pragma omp single
	{
            for (int ii=0; ii<nbx; ii++) {
		for (int jj=0; jj<nby; jj++) {
		#pragma omp task depend(in: aux_depend[ii-1][jj], aux_depend[ii][jj-1]) depend(out: aux_depend[ii][jj]) private(diff, unew)
     		    {
    			double omp_sum = 0.0;			
		        for (int i = 1 + ii * bx; i <= min((ii + 1) * bx, sizex - 2); i++) {
		             for (int j = 1 + jj * by; j <= min((jj + 1) * by, sizey - 2); j++) {
	            		unew= 0.25 * (    u[ i*sizey	+ (j-1) ]+  // left
				      u[ i*sizey	+ (j+1) ]+  // right
				      u[ (i-1)*sizey	+ j     ]+  // top
				      u[ (i+1)*sizey	+ j     ]); // bottom
	            		diff = unew - u[i*sizey+ j];
				omp_sum += diff * diff;
//	            		sum += diff * diff; 
	            		u[i*sizey+j]=unew;
			     }
			}
			#pragma omp atomic
			sum += omp_sum;
		    }
                }
	    }
	}
    }

    return sum;
}
/* do-across defined below */ 
double relax_gauss(double *u, unsigned sizex, unsigned sizey) {
    double unew, diff, sum = 0.0;
    int nbx, bx, nby, by;

    nbx = NB; // Number of blocks in x direction
    bx = sizex / nbx + (sizex % nbx != 0); // Block size in x direction
    nby = NB; // Number of blocks in y direction
    by = sizey / nby + (sizey % nby != 0); // Block size in y direction

    // Parallel region with do-across pattern
    #pragma omp parallel for collapse(2) reduction(+:sum) ordered(2)
    for (int ii = 0; ii < nbx; ii++) {
        for (int jj = 0; jj < nby; jj++) {
            #pragma omp ordered depend(sink: ii - 1, jj) depend(sink: ii, jj - 1)
            {
                for (int i = 1 + ii * bx; i <= min((ii + 1) * bx, sizex - 2); i++) {
                    for (int j = 1 + jj * by; j <= min((jj + 1) * by, sizey - 2); j++) {
                        unew = 0.25 * (u[i * sizey + (j - 1)] +  // left
                                       u[i * sizey + (j + 1)] +  // right
                                       u[(i - 1) * sizey + j] +  // top
                                       u[(i + 1) * sizey + j]);  // bottom

                        diff = unew - u[i * sizey + j];
                        sum += diff * diff;
                        u[i * sizey + j] = unew;
                    }
                }
            }
            #pragma omp ordered depend(source) 
        }
    }

    return sum;
}
