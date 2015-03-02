//
//  BirdSprite.h
//  hellWorld
//
//  Created by gao.da on 1/23/15.
//
//

#ifndef hellWorld_BirdSprite_h
#define hellWorld_BirdSprite_h
#pragma once

#include "cocos2d.h"

typedef enum {
    ACTION_STATE_IDLE,
    ACTION_STATE_FLY,
    ACTION_STATE_DIE
} ActionState;

class BirdSprite : public cocos2d::Sprite {
public:
    BirdSprite();
    ~BirdSprite();
    
    static BirdSprite* getInstance();
    
    bool virtual init();
    
    bool createBird();
    
    void idle();
    
    void fly();
    
    void die();
    
    static int BIRD_TAG;
    static float BIRD_RAIDUS;
    
protected:
    static cocos2d::Animation *createAnimation(const char* fmt, int count, float fps);
    
private:
    static BirdSprite* sharedBirdSprite;
    
    bool changeState(ActionState state);
    cocos2d::Action* idleAction;
    cocos2d::Action* swingAction;
    ActionState currentStatus;
    
    std::string birdName;
    
    unsigned int isFirstTime;
    
};
#endif
