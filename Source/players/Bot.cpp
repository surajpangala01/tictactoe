
#include "Bot.h"
#include <cstdlib>  // For srand() and rand()
#include <ctime>
#include <stdio.h>

Bot::Bot()
{
    srand(time(0));  // Initialize random seed once
}

std::pair<int,int> Bot::botMove(char brd[3][3]) {
    // Check if the bot can win
    
    for(int i = 0;i  <3;i++){
        for(int j = 0;j < 3;j++){
            this->board[i][j] = brd[i][j];
        }
    }

    printBoard();
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'o';
                if (isWinningMove('o')) {
//                    board[i][j] = 'o';
                    return {i,j}; // Bot wins
                }
                board[i][j] = ' '; // Undo move
            }
        }
    }

    // Check if the player can win (block)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'x';
                if (isWinningMove('x')) {
//                    board[i][j] = 'o';  Block the player
                    return {i,j};
                }
                board[i][j] = ' '; // Undo move
            }
        }
    }
    // Make a random move if no winning or blocking moves are found
    return makeRandomMove();
}

bool Bot::isWinningMove(char symbol) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) || // Row
            (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol)) { // Column
            return true;
        }
    }
    // Check diagonals
    if ((board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) ||   // Main diagonal
        (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)) {   // Anti-diagonal
        return true;
    }
    return false;
}

std::pair<int,int> Bot::makeRandomMove() {
    srand(time(0)); // Seed for random number generator
    while (true) {
        int i = rand() % 3;
        int j = rand() % 3;
        if (board[i][j] == ' ') {
            board[i][j] = 'o';
            return{i,j};
        }
    }
    return {0,0};
}

void Bot::printBoard(){
    for(int i = 0;i<3;i++){
        for(int j = 0;j < 3;j++){
            printf("%c | ",board[i][j]);
        }
        printf("\n");
    }
}


