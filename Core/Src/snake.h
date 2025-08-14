/*
 * snake.h
 *
 *  Created on: Jan 17, 2025
 *      Author: esthe
 */

#ifndef SRC_SNAKE_H_
#define SRC_SNAKE_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
	int coordinates[80][2];
	int size;
	int direction;
	int food[2];
	int food_eaten;
	int game_status; // 0 is main menu, 1 is in progress, 2 is game over
} Snake;

void snake_init (Snake * snake);
void snake_update (Snake * snake);
void generate_food (Snake * snake);
int collision (Snake * snake); // 0 if bad 1 if good
void display (Snake * snake);
void clear_screen();

#endif /* SRC_SNAKE_H_ */
