/*
 * @File: tetris.c - deals with blocks
 * @Author: noct4a <noct4a@gmail.com>
 *
 * Copyright (c) 2024 Gao-Yu Tang
 */
#include "tetris.h"
#include "input.h"

/*
 * Game information 
 */
int user_key = ' ';
int cur_block_pos[4][2] = {0};
const int (*p_block_state) [4][2] = NULL;
int block_state = 0;
Block cur_block;
bool panel[ROW][COL] = {false};
int player_score = 0;
/* end of game information */

// the last pos of cur_block_pos
int last_pos[4][2] = {0};

/*
 * Basic block cordinates
 * the offset from the point(x, y)
 * */
const int basic_blocks[7][4][2] = 
{
	{{0,0}, {0,1}, {1,0}, {1,1}},
	{{0,0}, {0,1}, {0,2}, {0,3}},
	{{0,0}, {0,1}, {1,1}, {1,2}},
	{{0,0}, {0,1}, {-1,1}, {-1,2}},
	{{0,0}, {0,1}, {0,2}, {-1,2}},
	{{0,0}, {-1,0}, {0,1}, {0,2}},
	{{0,0}, {1,0}, {0,-1}, {0,1}}
};

/*
 * The transform array
 * spin in the direction of counterclockwise
 */
const int block_states_len[7] = {1, 2, 2, 2, 4, 4, 4};

const int I_states[2][4][2] =
{
	{{0,0}, {0,1}, {0,2}, {0,3}},
	{{0,0}, {-1,0}, {-2,0}, {-3,0}}
};

// the offset are test by trial and error, so it is ugly.
const int Z_states[2][4][2] = 
{
	{{0,0}, {0,-1}, {1,0}, {1,1}},
	{{0,2}, {-1,2}, {0,1}, {1,1}}
};

const int S_states[2][4][2] =
{
	{{0,0}, {0,1}, {1,-1}, {1,0}},	
	{{0,2}, {-1,1}, {0,1}, {1,2}}
};

const int L_states[4][4][2] =
{
	{{0,0}, {0,1}, {0,2}, {-1,2}},
	{{-1,1}, {-1,0}, {0,1}, {1,1}},
	{{0,0}, {0,-1}, {0,1}, {1,-1}},
	{{0,1}, {0,0}, {-1,0}, {-2,0}}
};

const int J_states[4][4][2] =
{
	{{-1,0}, {-2,0}, {-1,1}, {-1,2}},
	{{0,0}, {0,1}, {-1,1}, {-2,1}},
	{{-1,0}, {-1,-1}, {-1,1}, {0,1}},
	{{0,0}, {-1,0}, {-2,0}, {-2,1}}
};

const int T_states[4][4][2] = 
{
	{{0,0}, {1,0}, {0,-1}, {0,1}},
	{{0,1}, {-1,1}, {1,1}, {0,2}},
	{{0,0}, {-1,0}, {0,-1}, {0,1}},
	{{0,2}, {-1,2}, {1,2}, {0,1}}
};
/* end of transform array */

void init_panel()
{
	for(int j = 0; j < COL; j++)
		panel[25][j] = true;
}


void get_block()
{
	block_state = 0;

	srand(time(NULL));
	int r = rand() % 7;

	cur_block = r;

	switch(cur_block)
	{
		case O:
			break;
		case I:
			p_block_state = I_states;
			break;
		case Z:
			p_block_state = Z_states;
			break;
		case S:
			p_block_state = S_states;
			break;
		case L:
			p_block_state = L_states;
			break;
		case J:
			p_block_state = J_states;
			break;
		case T:
			p_block_state = T_states;
			break;
		default:
			printf("something wrong!\n");
			exit(1);
	}

	memcpy(cur_block_pos, basic_blocks[cur_block], sizeof(cur_block_pos));
	for(int i = 0; i < 4; i++)
	{
		cur_block_pos[i][0] += INIT_X;
		cur_block_pos[i][1] += INIT_Y;
	}
}

int left_adjust()
{
	int i = 0;
	int cur_x = 0, cur_y = 0;

	for(i = 0; i < 4; i++)
	{
		cur_x = cur_block_pos[i][0];
		cur_y = cur_block_pos[i][1];

		if(cur_y < 5 || panel[cur_x][cur_y])
		{
			for(i = 0; i < 4; i++)
				cur_block_pos[i][1] += 1;
			return 1;
		}
	}
	return 0;
}

int right_adjust()
{
	int i = 0;
	int cur_x = 0, cur_y = 0;

	for(i = 0; i < 4; i++)
	{
		cur_x = cur_block_pos[i][0];
		cur_y = cur_block_pos[i][1];

		if(cur_y > LIMIT_Y || panel[cur_x][cur_y])
		{
			for(i = 0; i < 4; i++)
				cur_block_pos[i][1] -= 1;
			return 1;
		}
	}
	return 0;
}

static bool available_spin()
{
	int i = 0;
	int cur_x = 0, cur_y = 0;

	for(i = 0; i < 4; i++)
	{
		cur_x = cur_block_pos[i][0];
		cur_y = cur_block_pos[i][1];

		if(cur_y < 5 || cur_y > LIMIT_Y || panel[cur_x][cur_y])
			return false;
	}
	return true;
}

void move_right()
{
	for(int i = 0; i < 4; i++)
		cur_block_pos[i][1] += 1;
	while(right_adjust()); // it's ok to call once, but such loop makes me feel calm.
}

void move_left()
{
	for(int i = 0; i < 4; i++)
		cur_block_pos[i][1] -= 1;
	while(left_adjust());
}

static void get_top_block_cord(int *x, int* y)
{
	int top_idx = 0;

	for(int i = 1; i < 4; i++)
	{
		if(cur_block_pos[i][1] < cur_block_pos[top_idx][1])
			top_idx = i;
	}

	*x = cur_block_pos[top_idx][0];
	*y = cur_block_pos[top_idx][1];
}


void spin()
{
	if(cur_block == O)
		return;

	int len = block_states_len[cur_block];

	int cord_x = 0, cord_y = 0;
	get_top_block_cord(&cord_x, &cord_y);

	block_state = (block_state + 1) % len;

	int old_block_pos[4][2] = {0};
	memcpy(old_block_pos, cur_block_pos, sizeof(old_block_pos));
	
	// change cordinate
	memcpy(cur_block_pos, p_block_state + block_state, sizeof(cur_block_pos));
	for(int i = 0; i < 4; i++)
	{
		cur_block_pos[i][0] += cord_x;
		cur_block_pos[i][1] += cord_y;
	}

	// boundary check
	if(!available_spin())
	{
		memcpy(cur_block_pos, old_block_pos, sizeof(cur_block_pos));
		block_state -= 1;
	}
	
	return;
}


static void draw_panel()
{
	int x = 0, y = 0;

	for(x = 5; x <= LIMIT_X; x++)
	{
		for(y = 5; y <= LIMIT_Y; y++)
		{
			if(panel[x][y])
				printf("\033[%d;%dH#",x ,y);
			else
				printf("\033[%d;%dH ",x, y);
		}
		printf("\n");
	}

}

bool eliminate()
{
	int i = 0;
	int j = 0;
	bool flag = true;

	for(i = 5; i <= LIMIT_X; i++)
	{
		flag = true;
		for(j = 5; j <= LIMIT_Y; j++)
		{
			if(!panel[i][j])
			{
				flag = false;
				break;
			}
		}

		if(flag)
		{
			int k = 0;
			for(k = i; k >= 5; k--)
			{
				memmove(&panel[k][0], &panel[k-1][0], sizeof(*panel));
			}
	
			player_score += 1;
			return true;
		}
	}

	return false;
}

void print_block()
{
	printf("\033[%d;%dH#\033[%d;%dH#\033[%d;%dH#"
			"\033[%d;%dH#\n", 
				cur_block_pos[0][0], cur_block_pos[0][1],
				cur_block_pos[1][0], cur_block_pos[1][1],
				cur_block_pos[2][0], cur_block_pos[2][1],
				cur_block_pos[3][0], cur_block_pos[3][1]);
}

void clean_block()
{
	printf("\033[%d;%dH \033[%d;%dH \033[%d;%dH "
			"\033[%d;%dH \n", 
				last_pos[0][0], last_pos[0][1],
				last_pos[1][0], last_pos[1][1],
				last_pos[2][0], last_pos[2][1],
				last_pos[3][0], last_pos[3][1]);

}

bool reach_ground()
{
	int i = 0;

	for(i = 0; i < 4; i++)
	{
		int cur_x = cur_block_pos[i][0];
		int cur_y = cur_block_pos[i][1];

		if(cur_x == LIMIT_X || panel[cur_x + 1][cur_y])
		{
			for(i = 0; i < 4; i++)
			{
				cur_x = cur_block_pos[i][0];
				cur_y = cur_block_pos[i][1];

				panel[cur_x][cur_y] = true;
			}
			print_block();

			return true;
		}	
	}
	return false;
}

void block_drop()
{
	while(!reach_ground())
	{
		print_block();

		memcpy(last_pos, cur_block_pos, sizeof(last_pos));
		for(int i = 0; i < 4; i++)
			cur_block_pos[i][0] += 1;

		usleep(500000);
		clean_block();
	}
}

static void p_game_over_msg()
{
	printf("\033[7;18HGAME OVER DAYO!\n");
}

static bool game_over()
{
	int j = 0;

	for(j = 0; j < COL; j++)
		if(panel[4][j])
		{
			p_game_over_msg();
			return true;
		}

	return false;
} 

static void score_msg()
{
	printf("\033[6;21H%d\n", player_score);	
}

// the drop function should be start round
void* start_round(void* arg)
{
	while(1)
	{
		score_msg();

		get_block();

		block_drop();

		bool flag = false;
		while(eliminate())
			flag = true;

		if(flag)
			draw_panel();

		if(game_over())
		{
			restore_input();
			exit(0);
		}
	}
}


void draw_boundary_and_info()
{
	printf("\033[2J");
	printf("\033[3;4H____________");
	printf("\033[25;4H^^^^^^^^^^^^");

	int i = 0;
	for(i = 4; i < ROW-1; i++)
	{
		printf("\033[%d;4H>", i);
		printf("\033[%d;15H<", i);
	}

	printf("\033[5;18HSCORE");
	printf("\033[21;18HTetris Game\033[22;18Hby noct4a");
}

void game_init()
{
	draw_boundary_and_info();
	init_panel();
}
