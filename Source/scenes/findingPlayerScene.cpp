#include "findingPlayerScene.h"
#include "MainScene.h"

using namespace ax;

bool FindingPlayerScene::init()
{
    // Ensure Layer initializes correctly
    if (!Scene::init()) {
        return false;
    }

    // Get the visible size and origin for positioning UI elements
    auto visibleSize = ax::Director::getInstance()->getVisibleSize();
    auto origin = ax::Director::getInstance()->getVisibleOrigin();

    // Add a background (optional)
     // Replace with your background image

    auto backGroundColor = LayerColor::create(Color4B(66, 245, 233,255));
    if(!backGroundColor){
        AXLOG("backGroundColor object not created");
        return false;
    }
    else{
        //std::cout<<"Hello"<<std::endl;
        this->addChild(backGroundColor,0);
    }

    // Add "Finding Player..." label
    auto label = Label::createWithTTF("Finding Player...", "fonts/Marker Felt.ttf", 36);
    if (label) {
        label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 50);
        this->addChild(label);
    }


    // auto cancelButton = MenuItemFont::create("Cancel", [](Object* sender) {
    //     AXLOG("Cancel button clicked!");
    //     // Handle cancel logic, such as going back to the menu scene
    //     auto menuScene = MainScene::create(); // Assuming MenuScene exists
    //     ax::Director::getInstance()->replaceScene(menuScene);
    // });

    // if (cancelButton) {
    //     cancelButton->setFontSizeObj(24);
    //     cancelButton->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 150);
    //     auto menu = Menu::create(cancelButton, nullptr);
    //     menu->setPosition(Vec2::ZERO);
    //     this->addChild(menu);
    // }

    return true;
}

void FindingPlayerScene::showServerError()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Create a semi-transparent overlay
    auto errorLayer = LayerColor::create(Color4B(0, 0, 0, 150)); // Dark overlay
    if (!errorLayer) {
        AXLOG("Error Layer creation failed!");
        return;
    }

    // "Server Not Reachable" label
    auto errorLabel = Label::createWithTTF("Server Not Reachable", "fonts/Marker Felt.ttf", 32);
    if (errorLabel) {
        errorLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
        errorLayer->addChild(errorLabel);
    }

    this->addChild(errorLayer, 10); // Add overlay on top

    // Delay for 1 second, then go to MainScene
    this->runAction(Sequence::create(
        DelayTime::create(1.0f), // Wait for 1 second
        CallFunc::create([]() {
            auto mainScene = MainScene::create();
            Director::getInstance()->replaceScene(mainScene);
        }),
        nullptr
    ));
}
