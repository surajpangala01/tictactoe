//
//  EndGameScene.h
//  tictactoe
//
//  Created by Suraj  P on 17/01/25.
//
#ifndef __END_GAME_SCENE_H__
#define __END_GAME_SCENE_H__

#include "axmol.h"

class EndGameScene : public ax::Scene{
    ax::Vec2 visibleSize = _director->getVisibleSize();
    ax::Vec2 origin      = _director->getVisibleOrigin();
    ax::Rect safeArea    = _director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;
public:
    static EndGameScene * create(char win){
        auto object = new EndGameScene();
        if(object){
            object -> init(win);
            return object;
        }
        else{
            delete object;
            return nullptr;
        }   
    }
    void init(char gameState);
    void restartButton();
    
};

#endif
