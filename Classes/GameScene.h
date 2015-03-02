//
//  GameScene.h
//  hellWorld
//
//  Created by gao.da on 1/22/15.
//
//

#ifndef hellWorld_GameScene_h
#define hellWorld_GameScene_h
#pragma once
#include "cocos2d.h"

class GameScene: public cocos2d::Scene {
public:
    GameScene();
    ~GameScene();
    
    bool virtual init();
    void restart();
    
    CREATE_FUNC(GameScene);
};

#endif
