
#inclde "init.h"
#include "boundary_val.h"

/**
 * The boundary values of the problem are set.
 */
void boundaryvalues(
                    int imax,
                    int jmax,
                    double **U,
                    double **V
                    );
{ // No-slip boundary conditions for U and V.

    for(int j=1, j<=jmax,j=j+1):
    { /*Eq.15*/
        U[0][j]=0.0;
        U[imax][j]=0.0;
      /*Eq.16*/
        V[0][j]=-V[1][j];
        V[imax+1][j]=V[imax][j];
    }
    
    for(int i=1, i<=imax,i=i+1):
    {/*Eq.15*/
        V[i][0]=0.0;
        V[i][jmax]=0.0;
    /*Eq.16*/
        U[i][0]=-U[i][1];
        U[i][jmax+1]=-U[i][jmax];
    }
    



}