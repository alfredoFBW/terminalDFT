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

double complex W_N2[FFT_SIZE/4];
double complex W_N4[FFT_SIZE/4][FFT_SIZE/4];

/* Initializes W_N and W_N2 */ 
void init_W()
{
	int k, n;
	for(k = 0; k < FFT_SIZE/4; k++){
		for(n = 0; n < FFT_SIZE/4; n++){
			W_N4[k][n] = cexp(-I*2*PI*k*n/(FFT_SIZE/4));	
		}
		W_N2[k] = cexp(-I*2*PI*k/(FFT_SIZE/2));
	}
}

/* Returns DFT in Y */
void dft(int *x_signal,  double *Y)
{
	double complex F_p, F_pp;
	double complex G_p, G_pp;
	double complex exp_aux; 
	int k, n;
	for(k = 0; k < FFT_SIZE/4; k++){
		F_p = F_pp = G_p  = G_pp = 0;
		for(n = 0; n < FFT_SIZE/4; n++){
			F_p  += x_signal[4*n]*W_N4[k][n];
			F_pp += x_signal[4*n + 2]*W_N4[k][n];
			G_p  += x_signal[4*n + 1]*W_N4[k][n];
			G_pp += x_signal[4*n + 3]*W_N4[k][n];
		}	
		F_pp    = F_pp*W_N2[k];
		G_pp    = G_pp*W_N2[k];
		exp_aux = cexp(-I*2*PI*k/FFT_SIZE);

		Y[k] 		    = cabs(F_p + F_pp + exp_aux*(G_p + G_pp));
		Y[k + FFT_SIZE/4]   = cabs(F_p - F_pp + exp_aux*(G_p - G_pp));
		Y[k + 2*FFT_SIZE/4] = cabs(F_p + F_pp - exp_aux*(G_p + G_pp));
		Y[k + 3*FFT_SIZE/4] = cabs(F_p - F_pp - exp_aux*(G_p - G_pp));
	}
}

