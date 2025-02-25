#include "axmol.h"
#include "players/Bot.h"

//#include<mutex>

/*
 modes:
    b - bot
    f - offline
    n - online
 */



class Board;
class Online;
// class Bot;

class GameController
{
public:

    bool init(char );
    static GameController* create(char);
    // CREATE_FUNC(GameController);
    void getMove(int row, int col);
    char mark;
    void boardLoad();
    Board* boardScene;
    void recieveMove(int row, int col);
    void* findingScenePtr;
    void serverError();
    void loadEndGameScene(char data);
private:
    char board[3][3];
    int countFilledCell;
    char playerTurn;

    Bot* bot;
    Online* online;


    char mode; // based on what user clicks in the MainScene
    // Player * player;
    bool checkWinner(int row, int col);
    int recursion(int cur_x,int cur_y, cxx14::pair<int,int> direction);
    bool preProcessMove(int row, int col);
    void botMoveWork();

    // void refresh();
    // static GameController instance
};
