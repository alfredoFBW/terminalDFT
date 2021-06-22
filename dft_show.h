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

#ifndef DFT_SHOW_H_
#define DFT_SHOW_H_

#include "dft_calc.h"			/* For FFT_SIZE */
#include <math.h>			/* For lround() */
#include <stdio.h>			/* printf */

#define MAX_ROW 		23 	/* Terminal rows 24 counting axis */
#define MAX_COL 		64
#define TERM_DIV		8	/* Divide terminal in 8 regions */
#define F_SAMP 			8000	
#define SAMP_CHAR 		(FFT_SIZE/(2*MAX_COL))
#define HZ_CHAR 		(F_SAMP/(2*MAX_COL))
#define HZ_PER_DIV 		(HZ_CHAR*TERM_DIV)
#define MULT_WEIGHT		6	/* To give more weight when rounding */

/*
 * Declarations
 *
 */


/* Consider the terminal as a matrix, which we'll call board */
extern char board[MAX_ROW][MAX_COL];

/* DFT depends on k X[k], we will divide the terminal with these k */ 
extern const int xaxis_k[TERM_DIV+1];

/* Calculate the number of chars that one double (%.1f) needs (10.0 -> 4)*/
int char_digits(double val);

/* Normalize between 0.0 and 1.0 */
void array_normalize(double *signal);

/* Round between 0 and MAX_ROW */
void array_norm_round(double *signal, long int *array_ret);

/* Uses ANSI scape codes */
void clean_board();


void prepare_board(long int *signal_show);


void show_board();

#endif /* DFT_SHOW_H_ */
