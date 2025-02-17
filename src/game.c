// Minesweeper main program code
// Author: Anton Havlovskyi

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MK60D10.h"
#include "game.h"
#include "rec.h"
#include "io.h"
#include "uart.h"
#include "rec.h"
#include "lptmr.h"
#include "mcg.h"

int chosen_x = 0;
int chosen_y = 0;

void delay(long long bound) {
  for(long long i = 0; i < bound; i++);
}

void initializeBoard(char board[SIZE][SIZE], char fillChar) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = fillChar;
        }
    }
}

void placeMines(char board[SIZE][SIZE]) {
    int minesPlaced = 0;

    while (minesPlaced < MINES) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;

        if (board[row][col] != '*') {
            board[row][col] = '*';
            minesPlaced++;
        }
    }
}

int countAdjacentMines(char board[SIZE][SIZE], int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int r = row + i;
            int c = col + j;
            if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board[r][c] == '*') {
                count++;
            }
        }
    }
    return count;
}

void revealCell(char board[SIZE][SIZE], char displayBoard[SIZE][SIZE], int row, int col, int *revealedCells) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || displayBoard[row][col] != '-') {
        return;
    }

    int mineCount = countAdjacentMines(board, row, col);
    displayBoard[row][col] = mineCount + '0';
    (*revealedCells)++;

    // If there are no adjacent mines, reveal surrounding cells recursively
    if (mineCount == 0) {
        displayBoard[row][col] = '0';
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i != 0 || j != 0) {
                    revealCell(board, displayBoard, row + i, col + j, revealedCells);
                }
            }
        }
    }
}

void printBoard(char board[SIZE][SIZE]) {
    printf("\n");
    bool k = false;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (j == chosen_x && i == chosen_y) {
                printf("{%c}", board[i][j]);
                k = true;
            }
            else if (k) {
                printf("%c", board[i][j]);
                k = false;
            }
            else {
                printf("|%c", board[i][j]);
            }
        }
        if (k) {
            printf("\n");
            k = false;
        }
        else {
            printf("|\n");
        }
    }
}

void placeFlag(char displayBoard[SIZE][SIZE], int row, int col) {
    if (displayBoard[row][col] == '-') {
        displayBoard[row][col] = 'F';
        printf("Flag placed at (%d, %d).\n", row, col);
    }
    else if (displayBoard[row][col] == 'F') {
        displayBoard[row][col] = '-';
        printf("Flag removed at (%d, %d).\n", row, col);
    }
    else {
        printf("Invalid position or already revealed cell.\n");
    }
}

void gameLoop (char board[SIZE][SIZE], char displayBoard[SIZE][SIZE], encoder_t *right_encoder, encoder_t *left_encoder) {
    int gameOver = 0;
    int revealedCells = 0;
    int totalCells = SIZE * SIZE - MINES;

    int was_right_rotated = 0;
	int was_right_pressed = 0;
	int was_left_rotated = 0;
	int was_left_pressed = 0;
	int top_treshold = 2;
	int bottom_treshold = -2;
	int was_right_rotated_count = 0;
	int was_left_rotated_count = 0;

    printf("\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r");
    while (!gameOver && revealedCells < totalCells) {

    	printBoard(displayBoard);

        printf("\n\rLeft encoder - vertical movement, press button to place flag\n\r");
        printf("Right encoder - horizontal movement, press button to uncover\n\r");

        // Wait for user input
        while (was_right_rotated == 0 && was_right_pressed == 0 && was_left_rotated == 0 && was_left_pressed == 0) {
			was_right_rotated = rec_process_rotation(
				rec_read(right_encoder->clk_pin),
				rec_read(right_encoder->dt_pin),
				&(right_encoder->last_state)
			);
			was_right_pressed = rec_read(right_encoder->sw_pin);

			was_left_rotated = rec_process_rotation(
				rec_read(left_encoder->clk_pin),
				rec_read(left_encoder->dt_pin),
				&(left_encoder->last_state)
			);
			was_left_pressed = rec_read(left_encoder->sw_pin);
        }

        printf("\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r");

        // Process user input
        if (was_right_rotated) {
			if (was_right_rotated == 1) {
				was_right_rotated_count++;
				if (was_right_rotated_count >= top_treshold) {
					if (chosen_x != SIZE - 1) { 	// Go right
						chosen_x++;
					}
					was_right_rotated_count = 0;
				}
			}
			else {
				was_right_rotated_count--;
				if (was_right_rotated_count <= bottom_treshold) {	// Go left
					if (chosen_x != 0) {
						chosen_x--;
					}
					was_right_rotated_count = 0;
				}
			}
			while (was_right_rotated) {
				was_right_rotated = rec_process_rotation(
					rec_read(right_encoder->clk_pin),
					rec_read(right_encoder->dt_pin),
					&(right_encoder->last_state)
				);
			}
		}
		if (was_right_pressed) {									// Uncover the tile
			if (board[chosen_y][chosen_x] == '*') {
				printf("BOOM! You hit a mine. Game over!\n\r");
				gameOver = 1;
			} else {
				int prevRevealedCells = revealedCells;
				revealCell(board, displayBoard, chosen_y, chosen_x, &revealedCells);

				if (revealedCells == prevRevealedCells) {
					printf("Cell already revealed.\n\r");
				}

				if (revealedCells == totalCells) {
					printf("Congratulations! You found all safe cells.\n\r");
					break;
				}
			}
			while(was_right_pressed) {
				was_right_pressed = rec_read(right_encoder->sw_pin);
			}
		}
		if (was_left_rotated) {
			if (was_left_rotated == 1) {
				was_left_rotated_count++;
				if (was_left_rotated_count >= top_treshold) {		// Go down
					if (chosen_y != SIZE - 1) {
						chosen_y++;
					}
					was_left_rotated_count = 0;
				}
			}
			else {
				was_left_rotated_count--;
				if (was_left_rotated_count <= bottom_treshold) {	// Go up
					if (chosen_y != 0) {
						chosen_y--;
					}
					was_left_rotated_count = 0;
				}
			}
			while (was_left_rotated) {
				was_left_rotated =  rec_process_rotation(
					rec_read(left_encoder->clk_pin),
					rec_read(left_encoder->dt_pin),
					&(left_encoder->last_state)
				);
			}
		}
		if (was_left_pressed) {									// Place flag
            placeFlag(displayBoard, chosen_y, chosen_x);
			while(was_left_pressed) {
				was_left_pressed = rec_read(left_encoder->sw_pin);
			}
		}
		delay(10000);
    }
}

void gameMain(encoder_t *right_encoder, encoder_t *left_encoder) {
    char board[SIZE][SIZE];
    char displayBoard[SIZE][SIZE];

    // Initialize boards
    initializeBoard(board, '-');
    initializeBoard(displayBoard, '-');

    // Initialize starting position
    chosen_x = SIZE / 2;
    chosen_y = SIZE / 2;

    // Place mines randomly on the board
    placeMines(board);

    gameLoop(board, displayBoard, right_encoder, left_encoder);

    // Reveal the mine locations at the end of the game
    printf("\nFinal Board:\n");
    printBoard(board);
}
