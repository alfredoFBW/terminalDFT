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

#include "dft_show.h"

extern char board[MAX_ROW][MAX_COL];
const int xaxis_k[TERM_DIV+1] = {0, 64, 128, 192, 256, 320, 384, 448, 512};

/* Calculate the number of chars that one double (%.1f) needs (10.0 -> 4)*/
int char_digits(double val)
{
	if(val < 10.0)
		return 3;
	else if(val < 100.0)
		return 4;
	else if(val < 1000.0)
		return 5;
	else if(val < 10000.0)
		return 6;
	else if(val < 100000.0)
		return 7;
	else
		return 0;	/* 7 is enough */
}

/* Normalize between 0.0 and 1.0 */
void array_normalize(double *signal)
{
	int k;
	double max = 0.0; /* Since abs(X(f)) >= 0 */
	for(k = 0; k < FFT_SIZE; k++){
		if(signal[k] > max)
			max = signal[k];
	}
	for(k = 0; k < FFT_SIZE; k++)
		signal[k] = signal[k]/max;
}

/* Round between 0 and MAX_ROW */
void array_norm_round(double *signal, long int *array_ret)
{
	int k;
	array_normalize(signal);
	for(k = 0; k < FFT_SIZE; k++){
		/* Multiply to represent it better */
		array_ret[k] = MULT_WEIGHT*lround((MAX_ROW-1)*signal[k]);
		if(array_ret[k] > MAX_ROW - 1)
			array_ret[k] = MAX_ROW - 1;
	}
}

/* Uses ANSI scape codes */
void clean_board()
{
	/*
	 * Clean and positionate the pointer
	 * to the begin of the terminal
	 */
	printf("\033[2J\033[1;1H");
}

void prepare_board(long int *signal_show)
{
	int k, j, p;
	for(k = 0; k < MAX_ROW; k++){
		board[k][0] = (k == (MAX_ROW-1)) ? '+' : '|';
		for(j = 1, p = 0; j < MAX_COL; j++, p+=TERM_DIV){
			board[k][j] = (signal_show[p] == (MAX_ROW -k -1)) ? 'o' : ' ';
			/*
			 *  With this we achieve stem plot
			 */
			if((k >= 1) && ((board[k-1][j] == 'o') || (board[k-1][j] == '|')))
				board[k][j] = '|';
			else if((k == MAX_ROW-2) && (signal_show[p] == 0))
				board[k][j] = 'o';	

			if(k == MAX_ROW - 1)
				board[k][j] = '-';
		}

	}
}


void show_board()
{
	int m, n, k;
	double freq = 0.0;
	for(k = 0; k < MAX_ROW; k++){
		for(n = 0; n < MAX_COL; n++){
			if((n >= 1) && ((board[k][n] == 'o') || (board[k][n] == '|'))){
				if(k <= 7)			/* Red */
					printf("\033[1;31m%c", board[k][n]);
				else if(k <= 14)		/* Yellow */
					printf("\033[1;33m%c", board[k][n]);
				else				/* Cyan */
					printf("\033[1;36m%c", board[k][n]);	
			}
			else{					/* White */
				printf("\033[1;37m%c", board[k][n]);
			}
		}	
		printf("\n");
	}
	for(n = 0, k = 0; n < MAX_COL; n+=TERM_DIV, k++){
		freq = (double)(F_SAMP*xaxis_k[k]/FFT_SIZE);
		freq = freq/1000.0;
		printf("%.1f", freq);
		for(m = 0; m < TERM_DIV-char_digits(freq); m++){
			printf("%c", ' ');
		}
		
	}
	printf("[KHz]\n");
}



