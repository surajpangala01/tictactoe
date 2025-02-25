//
//  EndGameScene.cpp
//  ConvertUTF
//
//  Created by Suraj  P on 18/01/25.
//

#include "EndGameScene.h"
#include "MainScene.h"
#include "BoardScene.h"
#include "ui/UIButton.h"
using namespace ax;

void EndGameScene::init(char gameState)
{
    if (!Scene::init())
    {
        return;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Background Color
    auto backGroundColor = LayerColor::create(Color4B(66, 245, 233, 255));
    this->addChild(backGroundColor, 0);

    // Winner Mark Sprite (centered)
    if (gameState == 'x' || gameState == 'o')
    {
        auto sprite = Sprite::create(gameState == 'x' ? "X.png" : "O.png");
        if (sprite)
        {
            sprite->setAnchorPoint(Vec2(0.5, 0.5));
            sprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
            sprite->setContentSize(Size(200, 200));
            this->addChild(sprite, 2);
        }
    }
    else if(gameState == ' '){
        auto sprite = Sprite::create("X.png");

        if (sprite)
        {
            sprite->setAnchorPoint(Vec2(0.5, 0.5));
            sprite->setPosition(Vec2(origin.x + visibleSize.width / 2 - 120, origin.y + visibleSize.height / 2));
            sprite->setContentSize(Size(200, 200));
            this->addChild(sprite, 2);
        }
        sprite = Sprite::create("O.png");
        if (sprite)
        {
            sprite->setAnchorPoint(Vec2(0.5, 0.5));
            sprite->setPosition(Vec2(origin.x + visibleSize.width / 2 + 120, origin.y + visibleSize.height / 2));
            sprite->setContentSize(Size(200, 200));
            this->addChild(sprite, 2);
        }
    }
    // Result Label (bottom)
    std::string resultText = (gameState == 'x' || gameState == 'o')? "WINNER!" : "DRAW!";
    auto resultLabel = Label::createWithTTF(resultText, "fonts/Marker Felt.ttf", 60);
    resultLabel->setTextColor(Color4B::WHITE);
    resultLabel->setAnchorPoint(Vec2(0.5, 0));
    resultLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y +visibleSize.height / 2 - 200.0f));
    this->addChild(resultLabel, 2);

    // Restart Button
    auto restartButton = ui::Button::create("home.png","home.png");
    restartButton->setAnchorPoint(Vec2(0.5, 0.5));
    restartButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 100.0f));
    restartButton->setScale(0.1f);
//    restartButton->Node::setContentSize(Vec2(50.0f,50.0f));
    restartButton->addClickEventListener([gameState](Object* sender) {
        AXLOG("Restart button clicked!");
        Scene* scene = nullptr;

        scene = MainScene::create(); // Restart the board for the same mode

        if (scene)
        {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
        }
    });
    this->addChild(restartButton, 2);



    scheduleUpdate();
}
