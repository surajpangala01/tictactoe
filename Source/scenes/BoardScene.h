//
//  Board.h
//  tictactoe
//
//  Created by Suraj  P on 20/01/25.
//

#include "axmol.h"
//class GameController;
//#include "Controller.h"

class GameController;

struct ButtonData{
    int row;
    int col;
};

class Board : public ax::Scene{
private:
    GameController* controller;
    ax::Vec2 visibleSize;
    ax::Vec2 origin      ;
    ax::Rect safeArea   ;
    ax::Vec2 safeOrigin ;
    // char playerTurn = 'x';
    // char board[3][3] = {{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}};
    // int countFilledCell;


public:
    
    bool init(GameController *);
    static Board* create();
    bool applyBackGround();
    bool applyBackGroundShade();
    bool createGrids();
    bool createButtons();
    void showMark(int row, int col, char playerTurn);
    // bool onButtonTouchFunc(Node *button,float x,float y,float cellWidth,ax::Touch* touch, ax::Event* event);
    // bool checkWinner(ButtonData* data);
    // int recursion(int cur_x, int cur_y, cxx14::pair<int, int> direction);
//    ~Board();
    static int scene_ID;
//    Board();

//    CREATE_FUNC(Board);
};
