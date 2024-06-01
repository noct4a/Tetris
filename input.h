/*
 * @File: input.h - header file for input.c
 * @Author: noct4a <noct4a@gmail.com>
 *
 * Copyright (c) 2024 Gao-Yu Tang
 */
#ifndef INPUT_H_
#define INPUT_H_

#include <termio.h> // to deal with user input

void set_input();

void restore_input();

void* user_input(void* arg);


#endif
