/*
 * @File: main.c - the entrance of tetris game
 * @Author: noct4a <noct4a@gmail.com>
 *
 * Copyright (c) 2024 Gao-Yu Tang
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "tetris.h"
#include "input.h"

			
int main()
{
	game_init();	

	pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, user_input, NULL);
    pthread_create(&thread2, NULL, start_round, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
	
	exit(0);
}
