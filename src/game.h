// Minesweeper main program code
// Author: Anton Havlovskyi

#ifndef SOURCES_GAME_H_
#define SOURCES_GAME_H_

#include "rec.h"

#define SIZE 5  // Define the size of the grid (5x5)
#define MINES 5 // Number of mines on the board

void gameMain(encoder_t *right_encoder, encoder_t *left_encoder);
void gameLoop (char board[SIZE][SIZE], char displayBoard[SIZE][SIZE], encoder_t *right_encoder, encoder_t *left_encoder);
void placeFlag(char displayBoard[SIZE][SIZE], int row, int col);
void printBoard(char board[SIZE][SIZE]);
void revealCell(char board[SIZE][SIZE], char displayBoard[SIZE][SIZE], int row, int col, int *revealedCells);
int countAdjacentMines(char board[SIZE][SIZE], int row, int col);
void placeMines(char board[SIZE][SIZE]);
void initializeBoard(char board[SIZE][SIZE], char fillChar);
void delay(long long bound);

#endif /* SOURCES_GAME_H_ */
