#include "sor.h"
#include <math.h>
#include "helper.h"

void sor(
  double omg,
  double dx,
  double dy,
  int    imax,
  int    jmax,
  double **P,
  double **RS,
  double *res,
  int **Flag,
  double presLeft,
  double presRight
) {
  int i,j;
  double rloc;
  double coeff = omg/(2.0*(1.0/(dx*dx)+1.0/(dy*dy)));

  int FluidCells = 0;
  /* SOR iteration */
  for(i = 1; i <= imax; i++) {
    for(j = 1; j<=jmax; j++) {
      if(Flag[i][j] == C_F){
	      P[i][j] = (1.0-omg)*P[i][j]
        	      + coeff*(( P[i+1][j]+P[i-1][j])/(dx*dx) + ( P[i][j+1]+P[i][j-1])/(dy*dy) - RS[i][j]);
	      FluidCells++;
      }
    }
  }

  /* compute the residual */
  rloc = 0;
  for(i = 1; i <= imax; i++) {
    for(j = 1; j <= jmax; j++) {
      if(Flag[i][j] == C_F){
	      rloc += ( (P[i+1][j]-2.0*P[i][j]+P[i-1][j])/(dx*dx) + ( P[i][j+1]-2.0*P[i][j]+P[i][j-1])/(dy*dy) - RS[i][j])*
        	      ( (P[i+1][j]-2.0*P[i][j]+P[i-1][j])/(dx*dx) + ( P[i][j+1]-2.0*P[i][j]+P[i][j-1])/(dy*dy) - RS[i][j]);
      }
    }
  }
  rloc = rloc/(FluidCells);
  rloc = sqrt(rloc);
  /* set residual */
  *res = rloc;


  /* set boundary values */
  for(i = 1; i <= imax; i++) {
    P[i][0] = P[i][1];
    P[i][jmax+1] = P[i][jmax];
    for(j=1; j <= jmax; j++) {
	switch(Flag[i][j]){
		case B_N: P[i][j] = P[i][j+1]; break;
		case B_O: P[i][j] = P[i+1][j]; break;
		case B_S: P[i][j] = P[i][j-1]; break;
		case B_W: P[i][j] = P[i-1][j]; break;

		case B_NO: P[i][j] = (P[i+1][j] + P[i][j+1])*0.5; break;
		case B_NW: P[i][j] = (P[i-1][j] + P[i][j+1])*0.5; break;
		case B_SO: P[i][j] = (P[i+1][j] + P[i][j-1])*0.5; break;
		case B_SW: P[i][j] = (P[i-1][j] + P[i][j-1])*0.5; break;
	}
    }
  }

  // if you want more arbitrary situations, if should be in forloop. For now, for the sake of speed, we leave it outside...
  if (Flag[0][jmax/2]==C_P){
      for(j = 1; j <= jmax; j++) { //Dirichlet BC for pressure
	   P[0][j] = presLeft*2.0 - P[1][j];
      }
  } else {
      for(j = 1; j <= jmax; j++) { //Neumann BC for pressure
 	   P[0][j] = P[1][j];
      }
  }
  if (Flag[imax+1][jmax/2]==C_P){
      for(j = 1; j <= jmax; j++) { //Dirichlet BC for pressure
 	   P[imax+1][j] = 2.0*presRight - P[imax][j];
      }
  } else {
      for(j = 1; j <= jmax; j++) { //Neumann BC for pressure
 	   P[imax+1][j] = P[imax][j];
      }
  }
}

