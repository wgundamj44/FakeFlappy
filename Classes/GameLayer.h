//
//  GameLayer.h
//  hellWorld
//
//  Created by gao.da on 1/23/15.
//
//

#ifndef hellWorld_GameLayer_h
#define hellWorld_GameLayer_h
#pragma once

#include "cocos2d.h"
#include "BirdSprite.h"
#include "OptionLayer.h"
#include <vector>

typedef enum _game_status {
    GAME_STATUS_READY = 1,
    GAME_STATUS_START,
    GAME_STATUS_OVER
} GameStatus;

class StatusDelegate {
public:
    virtual void onGameEnd() = 0;
    virtual void onGamePlaying(int score) = 0;
};

class GameLayer : public cocos2d::Layer, public OptionDelegate {
public:
    GameLayer();
    ~GameLayer();
    
    bool virtual init();
    
    void onTouch();
    
    CREATE_FUNC(GameLayer);
    
    CC_SYNTHESIZE(StatusDelegate*, delegator, Delegator);
    
    void setPhyWorld(cocos2d::PhysicsWorld* world) { this->world = world; }
    
    void update(float delta);
    
private:
    void rotateBird();
    void createPipes();
    int getRandomHeight();
    void checkHit();
    void birdSpriteFadeout();
    void birdSpriteRemove();
    
    
    cocos2d::PhysicsWorld* world;
    GameStatus gameStatus;
    int score;
    BirdSprite* bird;
    cocos2d::Node* groundNode;
    std::vector<Node*> pipes;
    cocos2d::Sprite *landSprite1, *landSprite2;
    cocos2d::SEL_SCHEDULE shiftLand;
    void scrollLand(float dt);
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void gameOver();
    
private:    
    const int VISIBLE_PIPE_COUNT = 2;
    const int PIPE_HEIGHT = 320;
    const int PIPE_WIDTH  = 52;
    const int PIPE_DISTANCE = 100;
    const int PIPE_INTERVAL = 180;
    const int WAIT_DISTANCE = 100;
    
    const int DOWN_PIPE_TAG = 100000;
    const int UP_PIPE_TAG = 100001;
    const int PASS_PIPE_TAG = 100002;
    const int NEW_PIPE_TAG  = 100003;
};


#endif
