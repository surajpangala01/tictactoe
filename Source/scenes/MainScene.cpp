/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainScene.h"
//#include "SinglePlayer.h"
//#include "WebSocketClass.h"
// #include "findingPlayerScene.h"
// #include "BoardScene.h"
#include "Controller.h"
#include <iostream>

using namespace ax;

static int s_sceneID = 1000;

// Utility function to print error message when files are not loaded
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Content/' in front of filenames in "
        "MainScene.cpp\n");
}

// Initialize the scene
bool MainScene::init()
{
    // Superclass initialization
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    // Add a "close" button to exit the game
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                           AX_CALLBACK_1(MainScene::menuCloseCallback, this));

    if (!closeItem || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = safeOrigin.x + safeArea.size.width - closeItem->getContentSize().width / 2;
        float y = safeOrigin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // Add background color
    auto backGroundColor = LayerColor::create(Color4B(66, 245, 233, 255));
    if (!backGroundColor)
    {
        problemLoading("Background color layer");
        return false;
    }
    else
    {
        this->addChild(backGroundColor, 0);
    }

    // Add "Tic Tac Toe" heading
    auto title = Label::createWithTTF("Tic-Tac-Toe", "fonts/Marker Felt.ttf", 50);
    title->setPosition(Vec2(safeOrigin.x + safeArea.size.width / 2,
                            safeOrigin.y + safeArea.size.height/2 + 100.0f));
    title->setTextColor(Color4B(0,0,0,255));
    this->addChild(title, 2);

    // Create "Play Offline" button
    auto playOffline = Label::createWithTTF("Play Offline", "fonts/Marker Felt.ttf", 30);
    playOffline->setPosition(Vec2(safeOrigin.x + safeArea.size.width / 2,
                                   safeOrigin.y + safeArea.size.height / 2));
    playOffline->setTextColor(Color4B(0,0,0,255));
    this->addChild(playOffline, 2);

    auto touchListenerOffline = EventListenerTouchOneByOne::create();
    touchListenerOffline->onTouchBegan = AX_CALLBACK_2(MainScene::callbackForOfflineGame, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListenerOffline, playOffline);

    //Create "Play Computer" button
    auto playWithComputer = Label::createWithTTF("Play Computer", "fonts/Marker Felt.ttf", 30);
    playWithComputer->setPosition(Vec2(safeOrigin.x + safeArea.size.width / 2,
                                        safeOrigin.y + safeArea.size.height / 2.0f - 50.0f));
    playWithComputer->setTextColor(Color4B(0,0,0,255));
    this->addChild(playWithComputer, 2);

    auto touchListenerComputer = EventListenerTouchOneByOne::create();
    touchListenerComputer->onTouchBegan = AX_CALLBACK_2(MainScene::callbackForPlayWithComputer, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListenerComputer, playWithComputer);

    // Create "Play Online" button
    auto playOnline = Label::createWithTTF("Play Online", "fonts/Marker Felt.ttf", 30);
    playOnline->setPosition(Vec2(safeOrigin.x + safeArea.size.width / 2,
                                  safeOrigin.y + safeArea.size.height / 2 - 100.0f));
    playOnline->setTextColor(Color4B(0,0,0,255));
    this->addChild(playOnline, 2);

    auto touchListenerOnline = EventListenerTouchOneByOne::create();
    touchListenerOnline->onTouchBegan = AX_CALLBACK_2(MainScene::callbackForOnlineGame, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListenerOnline, playOnline);

    scheduleUpdate();
    return true;
}

bool MainScene::callbackForOfflineGame(Touch* touch, Event* event)
{
    auto target = static_cast<Node*>(event->getCurrentTarget());

    if (target->getBoundingBox().containsPoint(touch->getLocation()))
    {

        auto controller = GameController::create('f');
        return true;
    }
    return false;
}

bool MainScene::callbackForPlayWithComputer(Touch* touch, Event* event)
{
    auto target = static_cast<Node*>(event->getCurrentTarget());

    if (target->getBoundingBox().containsPoint(touch->getLocation()))
    {
        auto controller = GameController::create('b');
        return true;
    }
    return false;
}

bool MainScene::callbackForOnlineGame(Touch* touch, Event* event)
{
    auto target = static_cast<Node*>(event->getCurrentTarget());

    if (target->getBoundingBox().containsPoint(touch->getLocation()))
    {
        auto controller = GameController::create('n');
        //Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
        return true;
    }
    return false;
}

void MainScene::menuCloseCallback(ax::Object* sender)
{
    _director->end();
}

MainScene::MainScene()
{
    _sceneID = ++s_sceneID;
    AXLOGD("Scene: ctor: #%d", _sceneID);
}

MainScene::~MainScene()
{
    AXLOGD("~Scene: dtor: #%d", _sceneID);

    if (_touchListener)
        _eventDispatcher->removeEventListener(_touchListener);
}
