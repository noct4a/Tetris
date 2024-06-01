/*
 * @File: tetris.h - header file for tetris.c
 * @Author: noct4a <noct4a@gmail.com>
 *
 * Copyright (c) 2024 Gao-Yu Tang
 */ 


#ifndef TETRIS_H_
#define TETRIS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

/* actually used row:5~24, col:5~14 */
#define ROW 26
#define COL 15

#define INIT_X 4
#define INIT_Y 9
#define LIMIT_X 24
#define LIMIT_Y 14

typedef enum blocks
{
	O = 0,
	I,
	Z,
	S,
	L,
	J,
	T,
}Block;

void game_init();

void init_panel();

void get_block();

int left_adjust();

void move_left();

int right_adjust();

void move_right();

void spin();

bool eliminate();

void print_block();

void clean_block();

bool reach_ground();

void block_drop();

void* start_round(void* arg);

void draw_boundary_and_info();

#endif
