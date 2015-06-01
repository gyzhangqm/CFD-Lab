#include "helper.h"
#include "init.h"
#include <math.h>

int read_parameters( const char *szFileName,       /* name of the file */
                    double *Re,                /* reynolds number   */
                    double *UI,                /* velocity x-direction */
                    double *VI,                /* velocity y-direction */
                    double *PI,                /* pressure */
                    double *GX,                /* gravitation x-direction */
                    double *GY,                /* gravitation y-direction */
                    double *t_end,             /* end time */
                    double *xlength,           /* length of the domain x-dir.*/
                    double *ylength,           /* length of the domain y-dir.*/
                    double *dt,                /* time step */
                    double *dx,                /* length of a cell x-dir. */
                    double *dy,                /* length of a cell y-dir. */
                    int  *imax,                /* number of cells x-direction*/
                    int  *jmax,                /* number of cells y-direction*/
                    double *alpha,             /* uppwind differencing factor*/
                    double *omg,               /* relaxation factor */
                    double *tau,               /* safety factor for time step*/
                    int  *itermax,             /* max. number of iterations  */
		                               /* for pressure per time step */
                    double *eps,               /* accuracy bound for pressure*/
		    double *dt_value,          /* time for output */

		    int *wl,			/*initial boundary for left wall*/
		    int *wr,			/*initial boundary for right wall*/
		    int *wt,			/*initial boundary for top wall*/
		    int *wb,			/*initial boundary for bottom wall*/
		    char *problem)		/*problem to solve*/

{
   READ_DOUBLE( szFileName, *xlength );
   READ_DOUBLE( szFileName, *ylength );

   READ_DOUBLE( szFileName, *Re    );
   READ_DOUBLE( szFileName, *t_end );
   READ_DOUBLE( szFileName, *dt    );

   READ_INT   ( szFileName, *imax );
   READ_INT   ( szFileName, *jmax );

   READ_DOUBLE( szFileName, *omg   );
   READ_DOUBLE( szFileName, *eps   );
   READ_DOUBLE( szFileName, *tau   );
   READ_DOUBLE( szFileName, *alpha );

   READ_INT   ( szFileName, *itermax );
   READ_DOUBLE( szFileName, *dt_value );

   READ_DOUBLE( szFileName, *UI );
   READ_DOUBLE( szFileName, *VI );
   READ_DOUBLE( szFileName, *GX );
   READ_DOUBLE( szFileName, *GY );
   READ_DOUBLE( szFileName, *PI );

   READ_INT   ( szFileName, *wl );
   READ_INT   ( szFileName, *wr );
   READ_INT   ( szFileName, *wt );
   READ_INT   ( szFileName, *wb );

   READ_STRING( szFileName, problem );

   *dx = *xlength / (double)(*imax);
   *dy = *ylength / (double)(*jmax);

   return 1;
}

/**
 * The arrays U,V and P are initialized to the constant values UI, VI and PI on
 * the whole domain.
 */
void init_uvp(
  double UI,
  double VI,
  double PI,
  int imax,
  int jmax,
  double **U,
  double **V,
  double **P
) {
	init_matrix(U, 0, imax+1, 0, jmax+1, UI);
	init_matrix(V, 0, imax+1, 0, jmax+1, VI);
	init_matrix(P, 0, imax+1, 0, jmax+1, PI);
}

/**
 * The integer array Flag is initialized to constants C_F for fluid cells and C_B
 * for obstacle cells as specified by the parameter problem.
 */
void init_flag(
  char *problem,
  int imax,
  int jmax,
  int **Flag
) {
	int i,j;
	//initialisation to C_F and C_B
	int temp;
	int **Pic = read_pgm(problem);

	init_imatrix(Flag, 0, imax+1, 0, jmax+1, C_F); //C_F value for fluid cells are temporarily set as 100

	for (int i=1; i<imax+1; i++){
		for (int j=1; j<jmax+1; j++){
			temp = min(Pic[i][j]*pow(2,4) + Pic[i+1][j]*pow(2,3) + Pic[i-1][j]*pow(2,2) + Pic[i][j-1]*2 + Pic[i][j+1], 16);
			if (temp == 3 || temp ==7 || (temp > 10 && temp < 15))
				ERROR("Invalid geometry! Forbidden boundary cell found.\n")
			Flag[i][j] = temp;
		}
	}


	//set outer boundary
	for (i=0; i<=imax+1; i++){
		Flag[i][0] = C_B;
		Flag[i][jmax+1] = C_B;
	}
	for (j=0; j<=jmax+1; j++){
		Flag[0][j] = C_B;
		Flag[imax+1][j] = C_B;
	}

}