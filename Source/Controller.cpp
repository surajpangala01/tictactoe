#include "Controller.h"
#include "scenes/EndGameScene.h"
#include "scenes/BoardScene.h"
#include "players/Online.h"
#include <utility>
#include "scenes/findingPlayerScene.h"

// #include ""

using namespace ax;

GameController* GameController::create(char mode)
{
    auto boardScene = new GameController();
    if (boardScene && boardScene->init(mode))
    {
        return boardScene;
    }
    delete boardScene;
    boardScene = nullptr;
    return boardScene;
}

bool GameController::init(char mode)
{
    this->mode = mode;


    if(mode == 'f'){
        boardLoad();
    }
    else if (mode == 'b')
    {
        bot = new Bot();
        boardLoad();
    }
    else if(mode == 'n'){
        online = new Online(this);

        auto scene = FindingPlayerScene::create();
        ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f,scene));
        findingScenePtr = scene;
        if(!online->init()){
            AXLOG("Couldn't connect to server");
            scene->showServerError();
            return false;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = ' ';
        }
    }
    playerTurn      = 'x';
    countFilledCell = 0;
    return true;
}

void GameController::boardLoad(){
    boardScene = Board::create();
    boardScene->init(this);
    Director::getInstance()->replaceScene(ax::TransitionFade::create(1.0f, boardScene));
}

int GameController::recursion(int cur_x, int cur_y, cxx14::pair<int, int> direction)
{
    if (cur_x < 0 || cur_x > 2 || cur_y < 0 || cur_y > 2 || board[cur_x][cur_y] != playerTurn)
    {
        return 0;  // Base case: out of bounds or not player's piece
    }
    // AXLOG("row = %d, col = %d, board[row][col] = %c playerTurn = %c", cur_x, cur_y, board[cur_x][cur_y], playerTurn);
    //  Move to the next cell in the given direction
    int new_x = cur_x + direction.first;
    int new_y = cur_y + direction.second;

    // Recursive call, adding 1 for the current cell
    return 1 + recursion(new_x, new_y, direction);
}

bool GameController::checkWinner(int row, int col)
{

    // Define directions for checking lines
    cxx14::pair<int, int> directions[8] = {
        {1, -1},   // top-left
        {-1, 1},   // bottom-right
        {0, -1},   // left
        {0, 1},    // right
        {-1, -1},  // bottom-left
        {1, 1},    // top-right
        {-1, 0},   // bottom
        {1, 0}     // top
    };

    for (int i = 0; i < 4; i++)
    {
        int count = 1;  // Start count at 1 (current piece)

        // Check in one direction
        auto direction = directions[2 * i];
        count += recursion(row + direction.first, col + direction.second, direction);

        // Check in the opposite direction
        direction = directions[2 * i + 1];
        count += recursion(row + direction.first, col + direction.second, direction);

        if (count >= 3)
        {
            AXLOG("Player %c is the winner", playerTurn);

            return true;
        }
    }

    return false;
}

void GameController::getMove(int row, int col)
{
    if (!preProcessMove(row, col))
    {
        return;
    }
    countFilledCell++;
    board[row][col] = playerTurn;
    boardScene->showMark(row, col, playerTurn);
    if(mode == 'n'){
        if(!online){
            AXLOG("online is nullptr");
        }
        else{
            online -> sendMove(row,col);
        }
    }
    bool isThereWinner = checkWinner(row, col);

    if (isThereWinner)
    {
        auto newScene = EndGameScene::create(playerTurn);
        ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(1.0f, newScene));
        return;
    }
    if (countFilledCell == 9)
    {
        auto newScene = EndGameScene::create(' ');
        ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(1.0f, newScene));
        return;
    }

    if (playerTurn == 'x')
        playerTurn = 'o';
    else
        playerTurn = 'x';

    if (mode == 'b')
    {
        boardScene->scheduleOnce([this](float) {
            this->botMoveWork();
        }, 0.5f, "bot_move_delay");
    }
}

bool GameController::preProcessMove(int row, int col)
{
    if (board[row][col] != ' ')
    {
        return false;
    }
    if (mode == 'b')
    {
        if (playerTurn != 'x')
            return false;
    }
    else if(mode == 'n'){
        if(playerTurn != mark)return false;
    }
    return true;
}

void GameController::recieveMove(int row, int col){
    countFilledCell++;
    board[row][col] = playerTurn;
    boardScene->showMark(row, col, playerTurn);
    if(mode == 'n'){
        if(!online){
            AXLOG("online is nullptr");
        }
        else{
            online -> sendMove(row,col);
        }
    }
    bool isThereWinner = checkWinner(row, col);

    if (isThereWinner)
    {
        loadEndGameScene(playerTurn);
        return;
    }
    if (countFilledCell == 9)
    {
        loadEndGameScene(' ');
        return ;
    }

    if (playerTurn == 'x')
        playerTurn = 'o';
    else
        playerTurn = 'x';

}

void GameController::botMoveWork()
{
    auto botCell = bot->botMove(board);
    int botRow   = botCell.first;
    int botCol   = botCell.second;
    AXLOG("%d %d", botRow, botCol);
    board[botRow][botCol] = 'o';
    boardScene->showMark(botRow, botCol, playerTurn);
    bool isThereWinner = checkWinner(botRow, botCol);
    countFilledCell++;

    if (isThereWinner)
    {
        auto newScene = EndGameScene::create(playerTurn);
        ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(1.0f, newScene));
        return;
    }
    if (countFilledCell == 9)
    {
        auto newScene = EndGameScene::create(' ');
        ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(1.0f, newScene));
        return;
    }
    if (playerTurn == 'x')
        playerTurn = 'o';
    else
        playerTurn = 'x';
}

void GameController::serverError(){
    auto * scene = static_cast<FindingPlayerScene*>(findingScenePtr);
    scene->showServerError();
}

void GameController::loadEndGameScene(char data){
    auto newScene = EndGameScene::create(data);
        ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(1.0f, newScene));
        // return;
}
