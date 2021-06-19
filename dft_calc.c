/*
 *  terminalDFT. DFT terminal computation and plotting
 *
 *      Copyright (C) 2021  Alfredo Gonzalez Calvin
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "dft_calc.h"

#define MALLOC_FAILS 1

extern double complex W_N[FFT_SIZE/2];
extern double complex W_N2[FFT_SIZE/2][FFT_SIZE/2];

/* Initializes W_N and W_N2 */ 
void init_W()
{
	int k, n;
	for(k = 0; k < FFT_SIZE/2; k++){
		for(n = 0; n < FFT_SIZE/2; n++){
			W_N2[k][n] = cexp(-I*2*PI*k*n/(FFT_SIZE/2));	
		}
		W_N[k] = cexp(-I*2*PI*k/FFT_SIZE);
	}
}

/* Takes signal[n == even] and signal[n == odd] */
void take_evenodd(int *signal, int *x_even, int *x_odd)
{	
	int n = 0;
	while(n < FFT_SIZE/2){
		x_even[n] = signal[2*n];
		x_odd[n]  = signal[2*n + 1];
		n++;
	}
}

/* Returns DFT in Y */
int dft(int *x_even, int *x_odd, double *Y)
{
	double complex *F = calloc(FFT_SIZE/2,sizeof(double complex)); /*dft_ev*/
	double complex *G = calloc(FFT_SIZE/2,sizeof(double complex)); /*dft_od*/
	double complex *X = calloc(FFT_SIZE  ,sizeof(double complex)); /*dft*/
	if( (F == NULL) || (G == NULL) || (X == NULL) ){
		fprintf(stderr, "calloc() failed in dft()\n");
		return MALLOC_FAILS;
	}
	int k, n;
	for(k = 0; k < FFT_SIZE/2; k++){
		for(n = 0; n < FFT_SIZE/2; n++){
			F[k] += x_even[n]*W_N2[k][n];
			G[k] += x_odd[n]*W_N2[k][n];
		}	
		G[k] = W_N[k]*G[k];
	}
	for(k = 0; k < FFT_SIZE/2; k++){
		X[k] = F[k] + G[k];
		Y[k] = cabs(X[k]);
	}
	for(k = FFT_SIZE/2, n = 0; k < FFT_SIZE; k++, n++){
		X[k] = F[n] - G[n];
		Y[k] = cabs(X[k]);
	}
	free(F);
	free(G);
	free(X);
	return 0;
}

