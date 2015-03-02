//
//  BirdSprite.cpp
//  hellWorld
//
//  Created by gao.da on 1/23/15.
//
//

#include "BirdSprite.h"
#include "AtlasLoader.h"
USING_NS_CC;

BirdSprite::BirdSprite() {}

BirdSprite::~BirdSprite() {}

BirdSprite* BirdSprite::sharedBirdSprite = nullptr;

int BirdSprite::BIRD_TAG = 100;
float BirdSprite::BIRD_RAIDUS = 15.0f;

BirdSprite* BirdSprite::getInstance() {
    if (sharedBirdSprite == NULL) {
        sharedBirdSprite = new BirdSprite();
        if (!sharedBirdSprite->init()) {
            delete sharedBirdSprite;
            sharedBirdSprite = nullptr;
            CCLOG("ERROR: Could not init sharedBirdSprite");
        }
    }
    return sharedBirdSprite;
}

bool BirdSprite::init() {
    this->isFirstTime = 3;
    return true;
}

bool BirdSprite::createBird() {
    this->birdName = "TestBird";
    if (!Sprite::initWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("bird0_0"))) {
        return false;
    }
    Animation* animation = this->createAnimation("bird0_%d", 3, 10);
    Animate* animate = Animate::create(animation);
    this->idleAction = RepeatForever::create(animate);
    
    ActionInterval *up = CCMoveBy::create(0.4f, Point(0, 8));
    ActionInterval *upBack = up->reverse();
    this->swingAction = RepeatForever::create(Sequence::create(up, upBack, nullptr));
    
    return true;
}

void BirdSprite::idle() {
    if (changeState(ACTION_STATE_IDLE)) {
        this->runAction(this->idleAction);
        this->runAction(this->swingAction);
    }
}

void BirdSprite::fly() {
    if (changeState(ACTION_STATE_FLY)) {
        this->stopAction(this->idleAction);
        this->getPhysicsBody()->setGravityEnable(true);
    }
}

void BirdSprite::die() {
    if(changeState(ACTION_STATE_DIE)) {
        this->stopAllActions();
    }
}

bool BirdSprite::changeState(ActionState state) {
    currentStatus = state;
    return true;
}

Animation* BirdSprite::createAnimation(const char *fmt, int count, float fps) {
    Animation* animation = Animation::create();
    animation->setDelayPerUnit(1 / fps);
    
    for(int i = 0; i < count ; i++) {
        const char *filename = String::createWithFormat(fmt, i)->getCString();
        SpriteFrame *frame = AtlasLoader::getInstance()->getSpriteFrameByName(filename);
        animation->addSpriteFrame(frame);
    }
    return animation;
}

