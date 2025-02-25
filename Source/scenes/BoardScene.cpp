//
//  Untitled.h
//
//  Created by Suraj  P on 20/01/25.
//
#include "BoardScene.h"
// #include "MainScene.h"
#include "EndGameScene.h"
#include "Controller.h"
int Board::scene_ID = 0;
using namespace ax;



bool Board::init(GameController* controller)
{
    if (!Scene::init())
    {
        return false;
    }
//    countFilledCell  = 0;
    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;
    this->controller = controller;
    if (!applyBackGround()){
        AXLOG("Error Applying Background");
        return false;
    }

    if (!applyBackGroundShade()) {
        AXLOG("Error Applying Background shade");
        return false;
    }
    if (!createGrids()) {
        AXLOG("Error Applying grids");
        return false;
    }
    if (!createButtons()){
        AXLOG("Error creating Buttons");
        return false;
    }
    return true;
}

Board* Board::create(){
    auto instance = new Board();
    if(instance){
        return instance;
    }
    delete instance;
    instance = nullptr;
    return instance;
}

bool Board::applyBackGround()
{
    auto backGroundColor = LayerColor::create(Color4B(66, 245, 233, 255));
    if (!backGroundColor)
    {
        AXLOG("Some error in applybackground()");
        return false;
    }
    // std::cout<<"Hello"<<std::endl;
    this->addChild(backGroundColor, 0);
    return true;
}

bool Board::applyBackGroundShade()
{
    // Background Shade
    auto rectBar = DrawNode::create();
    if (!rectBar)
    {
        AXLOG("Some error in applybackgroundShade()");
        return false;
    }
    auto start_x = safeOrigin.x + safeArea.size.width / 5;
    auto start_y = safeOrigin.y + safeArea.size.height / 5;

    auto end_x               = safeOrigin.x + safeArea.size.width * 4 / 5;
    auto end_y               = safeOrigin.x + safeArea.size.height * 4 / 5;
    auto originLocation      = Vec2(start_x, start_y);
    auto destinationLocation = Vec2(end_x, end_y);

    auto fillColor = Color4B(66, 255, 233, 255);

    rectBar->drawSolidRect(originLocation, destinationLocation, fillColor, 0.0f, Color4B(0, 0, 0, 0));
    this->addChild(rectBar, 1);
    return true;
}

bool Board::createGrids()
{
    // Vertical Grid lines
    // thickness of grids is not yet relative to window size
    auto gridLine = DrawNode::create();
    if (!gridLine)
    {
        AXLOG("Some error in createGrids");
        return false;
    }
    auto start_x = safeOrigin.x + safeArea.size.width / 5;
    auto start_y = safeOrigin.y + safeArea.size.height / 5;

    auto end_x              = safeOrigin.x + safeArea.size.width * 4 / 5;
    auto end_y              = safeOrigin.x + safeArea.size.height * 4 / 5;
    float thickness         = 10;
    auto start_first_line_x = safeOrigin.x + safeArea.size.width * 2 / 5 - thickness / 2;
    auto end_first_line_x   = safeOrigin.x + safeArea.size.width * 2 / 5 + thickness / 2;
    //        auto gridLine = DrawNode::create();
    //        std::cout<<start_first_line_x<<"\t"<<end_first_line_x<<std::endl;
    gridLine->drawSolidRect(Vec2(start_first_line_x, start_y), Vec2(end_first_line_x, end_y), Color4B(0, 0, 0, 255),
                            0.5f, Color4B(0, 0, 0, 255));
    this->addChild(gridLine, 2);

    // float thickness = 10;
    auto start_second_line_x = safeOrigin.x + safeArea.size.width * 3 / 5 - thickness / 2;
    auto end_second_line_x   = safeOrigin.x + safeArea.size.width * 3 / 5 + thickness / 2;
    auto gridLine_second     = DrawNode::create();
    if (!gridLine_second)
    {
        return false;
    }
    gridLine_second->drawSolidRect(Vec2(start_second_line_x, start_y), Vec2(end_second_line_x, end_y),
                                   Color4B(0, 0, 0, 255), 0.5f, Color4B(0, 0, 0, 255));
    this->addChild(gridLine_second, 2);

    // Horizontal Grid Lines
    auto start_first_line_y  = safeOrigin.y + safeArea.size.height * 2 / 5 - thickness / 2;
    auto end_first_line_y    = safeOrigin.y + safeArea.size.height * 2 / 5 + thickness / 2;
    auto horizontalLineFirst = DrawNode::create();
    if (!horizontalLineFirst)
    {
        return false;
    }
    horizontalLineFirst->drawSolidRect(Vec2(start_x, start_first_line_y), Vec2(end_x, end_first_line_y),
                                       Color4B(0, 0, 0, 255), 0.5f, Color4B(0, 0, 0, 255));
    this->addChild(horizontalLineFirst, 2);

    // second one

    auto start_second_line_y  = safeOrigin.y + safeArea.size.height * 3 / 5 - thickness / 2;
    auto end_second_line_y    = safeOrigin.y + safeArea.size.height * 3 / 5 + thickness / 2;
    auto horizontalLineSecond = DrawNode::create();

    if (!horizontalLineSecond)
    {
        return false;
    }
    horizontalLineSecond->drawSolidRect(Vec2(start_x, start_second_line_y), Vec2(end_x, end_second_line_y),
                                        Color4B(0, 0, 0, 255), 0.5f, Color4B(0, 0, 0, 255));
    this->addChild(horizontalLineSecond, 2);
    return true;
}

bool Board::createButtons()
{
    auto cellWidth  = safeArea.size.width / 5;
    auto cellHeight = safeArea.size.height / 5;
    //    AXLOG("%f,%f\n", cellWidth, cellHeight);

    for (int i = 1; i <= 3; i++)
    {
        for (int j = 1; j <= 3; j++)
        {
            auto button = Node::create();
            auto x      = safeOrigin.x + i * cellWidth;
            auto y      = safeOrigin.y + j * cellHeight;

            // Ensure no overlap
            button->setAnchorPoint(Vec2(0, 0));
            button->setPosition(Vec2(x, y));
            button->setContentSize(Vec2(cellWidth, cellHeight));
            // button->setColor(Color3B(34, 134, 253));

            auto* data = new ButtonData{j - 1, i - 1};
            button->setUserData(data);

            auto touchListener = EventListenerTouchOneByOne::create();

            // Check that only the correct button responds
            touchListener->onTouchBegan = [this,button](Touch* touch, Event* event) -> bool {
                if(button->getBoundingBox().containsPoint(touch->getLocation())){
                    auto cellData = static_cast<ButtonData*>(button->getUserData());
                    controller->getMove(cellData->row,cellData->col);
                    return true;
                }
                return false;
            };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, button);
            this->addChild(button, 4);
        }
    }
    scheduleUpdate();
    return true;
}

void Board::showMark(int row, int col, char playerTurn){
    auto cellWidth = safeArea.size.width/5;
    auto cellHeight = safeArea.size.height/5;
    auto x = safeOrigin.x + cellWidth*(col+1);
    auto y = safeOrigin.y + cellHeight*(row+1);

    auto sprite = Sprite::create(playerTurn == 'x' ? "X.png":"O.png");

    if(!sprite){
        AXLOG("Some error loading sprites");
        return ;
    }
    sprite->setAnchorPoint(Vec2(0,0));
    sprite->setPosition(Vec2(x + 10.0f,y + 10.0f));
    sprite->setContentSize(Vec2(cellWidth/1.3,cellHeight/1.3));
    this->addChild(sprite,5);
}






