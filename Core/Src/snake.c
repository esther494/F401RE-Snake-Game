/*
 * snake.c
 *
 *  Created on: Jan 17, 2025
 *      Author: esthe
 */

#include "snake.h"
#include "i2c-lcd.h"

void snake_init (Snake * snake) {
	snake->coordinates[0][0] = 4;
	snake->coordinates[0][1] = 3;
	for (int i = 1; i < 80; i++) {
		snake->coordinates[i][0] = 0;
		snake->coordinates[i][1] = 0;
	}
	snake->size = 1;
	snake->direction = 0;
	snake->food_eaten = 0;

	// randomly generate the first food
	int food_row = rand() % 4;
	int food_col = rand() % 20;

	snake->food[0] = food_row;
	snake->food[1] = food_col;
}

/*
 * 1. change direction of the head
 * 2. check if the head has gone out of the bound
 * 3. check if the head has eaten the food
 * 4. generate new food if it ate
 * 5. make the length longer by 1
 * 6. check if the head has collided with the body
 */
void snake_update (Snake * snake) {
	// 1. change direction of the head
  // this will be used to calculate the new head position
	int head[2];
	head[0] = snake->coordinates[0][0]; // row
	head[1] = snake->coordinates[0][1]; // col

	switch(snake->direction){
		case(0): // up
			head[0] = head[0] - 1;
			break;
		case(1): // down
			head[0] = head[0] + 1;
			break;
		case(2): // right
			head[1] = head[1] + 1;
			break;
		case(3): // left
			head[1] = head[1] - 1;
			break;
	}
	// 2. check if the head has gone out of the bound
	// if it goes out of bound it is game over
	if (head[0] < 0 || head[0] > 3 || head[1] < 0 || head[1] > 19) {
		snake->game_status = 2; // game over
	}

	// 3. check if the head has eaten the food
	generate_food(snake);

	if (snake->size > 1) {
		int length = snake->size;
		int last_coord[2];
		last_coord[0] = snake->coordinates[length-2][0];
		last_coord[1] = snake->coordinates[length-2][1];

		for (int i = length-1; i >= 1; i--) {
			snake->coordinates[i][0] = snake->coordinates[i-1][0];
			snake->coordinates[i][1] = snake->coordinates[i-1][1];
		}

		if (snake->food_eaten == 1) {
			snake->coordinates[length-1][0] = last_coord[0];
			snake->coordinates[length-1][1] = last_coord[1];
			snake->food_eaten = 0;
		}
	}

	snake->coordinates[0][0] = head[0];
	snake->coordinates[0][1] = head[1];

	// 6. check if the head has collided with the body
  if (collision(snake, head[0], head[1]) == 0){
    snake->game_status = 2;
  }
}

int collision (Snake * snake, int row, int col) {
	for(int i = 1; i < snake->size; i++) {
		if (row == snake->coordinates[i][0] &&
		    col == snake->coordinates[i][1]){
		    return 0;
		}
	}
	return 1;
}

// generate food and also check if the snake has eaten the food
void generate_food (Snake * snake) {
	// generate food if the food has been eaten
	if (snake->coordinates[0][0] == snake->food[0] && snake->coordinates[0][1] == snake->food[1]) {
		int food_row, food_col;
	  do {
		  food_row = rand() % 4;
		  food_col = rand() % 20;
		} while (collision(snake, food_row, food_col) == 0);

		snake->food[0] = food_row;
		snake->food[1] = food_col;

		snake->food_eaten = 1;
		snake->size++;
	}
}

void display (Snake * snake) {
	clear_screen();
	// food
	lcd_cursor_pos(snake->food[0], snake->food[1]);
	lcd_send_string("-");

	// snake
	for (int i = 0; i < snake->size; i++) {
		int row = snake->coordinates[i][0];
		int col = snake->coordinates[i][1];
		lcd_cursor_pos(row, col);
		lcd_send_string(".");
	}
}

void clear_screen() {
	lcd_clear_display();
	// for some reason, my lcd won't clear the bottom right part of the screen
	// so i have to manually send a blank space to clear them
	for (int col = 9; col <= 19; col++) {
		lcd_cursor_pos(3, col);
		lcd_send_data(' ');
	}
}
