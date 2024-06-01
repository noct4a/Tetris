/*
 * @File: input.c - deals with users input
 * @Author: noct4a <noct4a@gmail.com>
 *
 * Copyright (c) 2024 Gao-Yu Tang
 */
#include "input.h"
#include "tetris.h"

extern int user_key;

struct termios newt, oldt;

void set_input()
{
	tcgetattr(0, &oldt);

	tcgetattr(0, &newt);
	newt.c_lflag = newt.c_lflag & ~(ICANON | ECHO);
	newt.c_cc[VMIN] = 1;
	newt.c_cc[VTIME] = 0;

	tcsetattr(0, TCSANOW, &newt);
}

void restore_input()
{
	tcsetattr(0, TCSANOW, &oldt);

	printf("\033[27;0H \n");
}


void* user_input(void* arg)
{
	set_input();
	while(1)
	{
		user_key = getchar();
		if(user_key == 'q')
		{
			printf("\033[7;18HQUIT!\n");
			break;
		}
		else if(user_key == 'd')
		{
			move_right();
		}
		else if(user_key == 'a')
		{
			move_left();
		}
		else if(user_key == 'w')
		{
			spin();
		}
		fflush(NULL);
	}

	restore_input();
	exit(0);
}


