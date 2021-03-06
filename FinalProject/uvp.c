#include "helper.h"
#include <math.h>
#include <omp.h>

#define fluid 4096 //pow2(2,12);
#define air 8192


struct fgh_params{
	double Re;
	double GX;
	double GY;
	double GZ;
	double alpha;
	double dt;
	double dx;
	double dy;
	double dz;
	int imax;
	int jmax;
	int kmax;
	double ***U;
	double ***V;
	double ***W;
	double ***F;
	double ***G;
	double ***H;
	int ***Flag;
};



void calculate_fgh(
		double Re,
		double GX,
		double GY,
		double GZ,
		double alpha,
		double dt,
		double dx,
		double dy,
		double dz,
		int imax,
		int jmax,
		int kmax,
		double ***U,
		double ***V,
		double ***W,
		double ***F,
		double ***G,
		double ***H,
		int ***Flag,
		struct p_pointer *PP1,
		int FluidCells

){
	double uijk, vijk, wijk, uujk, udjk, uiuk, uidk, uiju, uijd, viuk, vidk, viju, vijd, vujk, vdjk, wiju, wijd, wujk, wdjk, wiuk, widk, uduk, udju, vudk, vidu, wiud,wujd;
	double Dx = 1/dx, Dy = 1/dy, Dz = 1/dz;
	double ReInv = 1.0/Re;
	int i, j, k,s, fgg;

#pragma omp parallel for private(i,j,k,fgg,uijk, vijk, wijk, uujk, udjk, uiuk, uidk, uiju, uijd, viuk, vidk, viju, vijd, vujk, vdjk, wiju, wijd, wujk, wdjk, wiuk, widk, uduk, udju, vudk, vidu, wiud,wujd)
	for(s=0;s<FluidCells;s++){
		struct p_pointer *pp = &PP1[s];
		if((pp->u == 1) ||(pp->v == 1) || (pp->w == 1) ){
			i = pp->i;
			j = pp->j;
			k = pp->k;
			uijk = U[i][j][k];   vijk = V[i][j][k];   wijk = W[i][j][k];
			uujk = U[i+1][j][k]; viuk = V[i][j+1][k]; wiju = W[i][j][k+1];
			udjk = U[i-1][j][k]; vidk = V[i][j-1][k]; wijd = W[i][j][k-1];
			uiuk = U[i][j+1][k]; viju = V[i][j][k+1]; wujk = W[i+1][j][k];
			uidk = U[i][j-1][k]; vijd = V[i][j][k-1]; wdjk = W[i-1][j][k];
			uiju = U[i][j][k+1]; vujk = V[i+1][j][k]; wiuk = W[i][j+1][k];
			uijd = U[i][j][k-1]; vdjk = V[i-1][j][k]; widk = W[i][j-1][k];
			uduk = U[i-1][j+1][k]; udju = U[i-1][j][k+1];
			vudk = V[i+1][j-1][k]; vidu = V[i][j-1][k+1];
			wiud = W[i][j+1][k-1]; wujd = W[i+1][j][k-1];

			if (isfluid(Flag[i+1][j][k])){ // add this to ensure calculation is only on edges separating two fluid cells.
				F[i][j][k] = uijk + dt*(ReInv*((uujk-2*uijk+udjk)*Dx*Dx + (uiuk-2*uijk+uidk)*Dy*Dy + (uiju-2*uijk+uijd)*Dz*Dz) -
						0.25*Dx*(pow((uijk+uujk),2)-pow((udjk+uijk),2) + alpha*(fabs(uijk+uujk)*(uijk-uujk)-fabs(udjk+uijk)*(udjk-uijk))) -
						0.25*Dy*((vijk+vujk)*(uijk+uiuk)-(vidk+vudk)*(uidk+uijk) + alpha*(fabs(vijk+vujk)*(uijk-uiuk)-fabs(vidk+vudk)*(uidk-uijk))) -
						0.25*Dz*((wijk+wujk)*(uijk+uiju)-(wijd+wujd)*(uijd+uijk) + alpha*(fabs(wijk+wujk)*(uijk-uiju)-fabs(wijd+wujd)*(uijd-uijk))) +
						GX);
			}
			if (isfluid(Flag[i][j+1][k])){
				G[i][j][k] = vijk + dt*(ReInv*((vujk-2*vijk+vdjk)*Dx*Dx + (viuk-2*vijk+vidk)*Dy*Dy + (viju-2*vijk+vijd)*Dz*Dz) -
						0.25*Dy*(pow((vijk+viuk),2)-pow((vidk+vijk),2) + alpha*(fabs(vijk+viuk)*(vijk-viuk)-fabs(vidk+vijk)*(vidk-vijk))) -
						0.25*Dx*((uijk+uiuk)*(vijk+vujk)-(udjk+uduk)*(vdjk+vijk) + alpha*(fabs(uijk+uiuk)*(vijk-vujk)-fabs(udjk+uduk)*(vdjk-vijk))) -
						0.25*Dz*((wijk+wiuk)*(vijk+viju)-(wijd+wiud)*(vijd+vijk) + alpha*(fabs(wijk+wiuk)*(vijk-viju)-fabs(wijd+wiud)*(vijd-vijk))) +
						GY);
			}


			if (isfluid(Flag[i][j][k+1])){
				H[i][j][k] = wijk + dt*(ReInv*((wujk-2*wijk+wdjk)*Dx*Dx + (wiuk-2*wijk+widk)*Dy*Dy + (wiju-2*wijk+wijd)*Dz*Dz) -
						0.25*Dz*(pow((wijk+wiju),2)-pow((wijd+wijk),2) + alpha*(fabs(wijk+wiju)*(wijk-wiju)-fabs(wijd+wijk)*(wijd-wijk))) -
						0.25*Dx*((uijk+uiju)*(wijk+wujk)-(udjk+udju)*(wdjk+wijk) + alpha*(fabs(uijk+uiju)*(wijk-wujk)-fabs(udjk+udju)*(wdjk-wijk))) -
						0.25*Dy*((vijk+viju)*(wijk+wiuk)-(vidk+vidu)*(widk+wijk) + alpha*(fabs(vijk+viju)*(wijk-wiuk)-fabs(vidk+vidu)*(widk-wijk))) +
						GZ);
			}

		}
		else {

			//we have a boundary cell; rewrite with right boundary cond., depending on type of cell
			//printf("cell %d, %d, %d is boundary: \t", i, j, k);
			fgg = getboundarytype(Flag[i][j][k]);      //if no neighbours are fluid, we do nothing. thats ok, right? bcs we never need those values?
			if (fgg%4==1){ //U neighb. is fluid
				H[i][j][k] = W[i][j][k];
			} else if (fgg%4==2){  //D neighb. is fluid
				H[i][j][k-1] = W[i][j][k-1];
			}
			if (fgg>31){ // O neigh. is fluid
				F[i][j][k] = U[i][j][k];
			} else if (fgg>15) { // W neighb. is fluid
				F[i-1][j][k] = U[i-1][j][k];
			}
			if ( (fgg%16+1)/3==3 ) { //N neigh. is fluid
				G[i][j][k] = V[i][j][k];
			} else if ( (fgg%16+2)/3==2 ) { //S neigh. is fluid
				G[i][j-1][k] = V[i][j-1][k];
			}
		}
	}


#pragma omp parallel for private(j)
	for (i=1; i<imax+1; i++){
		for (j=1;j< jmax+1; j++){
			H[i][j][0]    = W[i][j][0];
			H[i][j][kmax] = W[i][j][kmax];
		}
	}

#pragma omp parallel for private(k)
	for (i=1; i<imax+1; i++){
		for (k=1; k<kmax+1; k++){
			G[i][0][k]   = V[i][0][k];
			G[i][jmax][k] = V[i][jmax][k];
		}
	}

   #pragma omp parallel for private(k)
	for (j=1; j<jmax+1; j++){
		for (k=1; k<kmax+1; k++){
			F[0][j][k] = U[0][j][k];
			F[imax][j][k] = U[imax][j][k];
		}
	}

}

void calculate_rs(
		double dt,
		double dx,
		double dy,
		double dz,
		int imax,
		int jmax,
		int kmax,
		double ***F,
		double ***G,
		double ***H,
		double ***RS,
		int ***Flag,
		struct p_pointer *PP1,
		int FluidCells
){
	int i, j, k,s;

#pragma omp parallel for private(i,j,k)
	for(s=0;s<FluidCells;s++){
		struct p_pointer *pp = &PP1[s];
		if(pp->p==1){
			i = pp->i;
			j = pp->j;
			k = pp->k;
			RS[i][j][k] = (
					(F[i][j][k] - F[i-1][j][k])/dx
					+ (G[i][j][k] - G[i][j-1][k])/dy
					+ (H[i][j][k] - H[i][j][k-1])/dz
			) / dt;
		}

	}
}

void calculate_dt(
		double Re,
		double tau,
		double *dt,
		double dx,
		double dy,
		double dz,
		int imax,
		int jmax,
		int kmax,
		double ***U,
		double ***V,
		double ***W
){
	double maxi1, maxi2, maxi3; // tmpT;
	/* we rewrite dt only if tau is nonnegative, else we do nothing */
	if(tau>=0){
		//tmp = 0.5*Re*(dx*dx*dy*dy*dz*dz)/(dx*dx*(dy*dy+dz*dz)+ dy*dy*dz*dz);
		//tmpT = Pr*tmp;  //if we add heat flow
		*dt = 0.5*Re*(1.0/(1.0/(dx*dx)+1.0/(dy*dy)+1.0/(dz*dz)));
		maxi1 = tmax(U, imax, jmax, kmax);
		maxi2 = tmax(V, imax, jmax, kmax);
		maxi3 = tmax(W, imax, jmax, kmax);
		if(maxi1>0){
			*dt =  fmin(*dt,dx/maxi1);
		}
		if(maxi2>0){
			*dt =  fmin(*dt,dy/maxi2);
		}
		if(maxi3>0){
			*dt =  fmin(*dt,dz/maxi3);
		}
		*dt = tau * (*dt);
	}
}


void calculate_uvw(
		double dt,
		double dx,
		double dy,
		double dz,
		int imax,
		int jmax,
		int kmax,
		double ***U,
		double ***V,
		double ***W,
		double ***F,
		double ***G,
		double ***H,
		double ***P,
		int ***Flag,
		struct p_pointer *PP1,
		int FluidCells
){







	int i, j, k,s;
	/*range of indices {1:(imax-1)}x{1:jmax}x{1:kmax} for F; {1:imax}x{1:(jmax-1)}x{1:kmax} for G, etc*/
    double dtx = dt/dx;
    double dty = dt/dy;
    double dtz = dt/dz;
	/*we only calculate U, V and W at the edges between two fluid cells*/


#pragma omp parallel for private(i,j,k)
	for(s=0;s<FluidCells;s++){
		struct p_pointer *pp = &PP1[s];
		i = pp->i;
		j = pp->j;
		k = pp->k;
		if(pp->u==1){
			U[i][j][k] = F[i][j][k] -  dtx* (P[i+1][j][k] - P[i][j][k]);
		}
		if(pp->v==1){
			V[i][j][k] = G[i][j][k] - dty * (P[i][j+1][k] - P[i][j][k]);
		}
		if(pp->w==1){
			W[i][j][k] = H[i][j][k] - dtz * (P[i][j][k+1] - P[i][j][k]);
		}
	}

}
