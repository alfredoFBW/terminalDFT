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

#ifndef DFT_CALC_H_
#define DFT_CALC_H_

#include <complex.h>		/* cabs() cexp() */
#include <stdlib.h>		/* malloc, free */
#include <stdio.h>		/* fprintf */

#define FFT_SIZE 1024 
#define PI 3.14159

/* 
 * Complex vector and complex matrix used
 * to compute the DFT using the base 2 
 * decimation algorithm (RADIX 4)
 */
extern double complex W_N2[FFT_SIZE/4];
extern double complex W_N4[FFT_SIZE/4][FFT_SIZE/4];


/* Initializes W_N2 and W_N4 */ 
void init_W();

/* Returns DFT in Y */
void dft(int *x_signal, double *Y);

#endif /* DFT_CALC_H_ */
