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


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "dft_calc.h"
#include "dft_show.h"

#define OPEN_CLOSE_FAILED -1
#define DFT_FAILED -2
#define EXEC_FAILED 3
#define SIGNAL_FAILED -4
#define RM_FAILED -5

#define AUDIOFILE_PATH "./audiofile"
#define AUDIOPROG_PATH "/usr/bin/arecord"

char *audio_command[] = {"arecord", "-q", "-c", "1", "-s", "1024",
			       "-r", "8000", "-f", "U8", "-t", "raw",
			       "./audiofile", NULL};
int end_proccess = 0;

/* Finish proccess when ctrl + c SIGINT(errupt) */
void sa_finish_int(int arg)
{
	end_proccess = 1;
}

/* Finish procces when sigterm */
void sa_finish_term(int arg)
{
	end_proccess = 1;
}

int main(int argc, char *argv[])
{
	FILE *file_audio;
	double dft_audio[FFT_SIZE];
	int audio_even[FFT_SIZE/2];
	int audio_odd[FFT_SIZE/2];
	int audio[FFT_SIZE];
	int ret = 0,k = 0;
	int child_status;
	pid_t pid_record;
	struct sigaction sa_int, sa_term;
	long int *audio_rounded = malloc(FFT_SIZE*sizeof(long int));

	if((file_audio = fopen(AUDIOFILE_PATH, "w+")) == NULL){
		fprintf(stderr, "fopen() error\n");
		return OPEN_CLOSE_FAILED;
	}
	sa_int.sa_handler = sa_finish_int; sa_term.sa_handler = sa_finish_term;
	sigemptyset(&sa_int.sa_mask);  sigemptyset(&sa_term.sa_mask);
	sa_term.sa_flags = 0;	       sa_term.sa_flags = 0;
	if((ret = sigaction(SIGINT, &sa_int, 0)) == -1){
		perror("sigaction failed() ");
		return SIGNAL_FAILED;
	}
	if((ret = sigaction(SIGTERM, &sa_term, 0)) == -1){
		perror("sigaction failed() ");
		return SIGNAL_FAILED;
	}

	init_W();
	while(!end_proccess){
		pid_record = fork();
		if(pid_record == 0){ 		/* Child */
			ret = execv(AUDIOPROG_PATH, audio_command);
			if(ret == -1){
				perror("exec() error ");
				exit(EXEC_FAILED);	/* Exit child on error */
			}
		}	
		else{		     		/* Parent */
			wait(&ret);
			child_status = WEXITSTATUS(ret);
			if(child_status == (0377 & EXEC_FAILED)){ /* see man 3 exit */
				fprintf(stderr, "child error\n");
				return -EXEC_FAILED;
			}
			/* Child closes it when he finishes recording */
			freopen(AUDIOFILE_PATH, "r", file_audio);
			for(k = 0; k < FFT_SIZE; k++)
				fread(audio + k, sizeof(u_int8_t), 1, file_audio);
		}
		take_evenodd(audio, audio_even, audio_odd);	
		if((ret = dft(audio_even, audio_odd, dft_audio)) != 0){
			fprintf(stderr, "malloc error dft()");
			return DFT_FAILED;
		}
		clean_board();
		array_norm_round(dft_audio, audio_rounded);
		prepare_board(audio_rounded);
		show_board();
	}
	clean_board();
	if((ret = fclose(file_audio)) != 0){
		perror("close() error ");
		return OPEN_CLOSE_FAILED;
	}	
	/* Remove audio file */
	if((ret = remove(AUDIOFILE_PATH)) == -1){
		perror("remove() file error ");
		return RM_FAILED;
	}
	free(audio_rounded);
	return 0;
}
