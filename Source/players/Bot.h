#include <utility>

// class GameController;

class Bot
{
public:
    std::pair<int, int> botMove(char board[][3]);
    Bot();

private:
    char board[3][3];
//    sched
    bool isWinningMove(char symbol);
    std::pair<int, int> makeRandomMove();
    // std::pair<int,int> makeRandomMove();
    void printBoard();
};
