//
//  GameScene.cpp
//  hellWorld
//
//  Created by gao.da on 1/22/15.
//
//

#include "GameScene.h"
#include "BackgroundLayer.h"
#include "GameLayer.h"
#include "StatusLayer.h"
USING_NS_CC;

GameScene::GameScene() {}

GameScene::~GameScene() {}

bool GameScene::init() {
    if (Scene::initWithPhysics()) {
        auto backgroundlayer = BackgroundLayer::create();
        if (backgroundlayer) {
            this->addChild(backgroundlayer);
        }
        
        this->getPhysicsWorld()->setGravity(Vect(0, -900));
        //this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
        
        auto gameLayer = GameLayer::create();
        gameLayer->setPhyWorld(this->getPhysicsWorld());
        this->addChild(gameLayer);
        
        auto statusLayer = StatusLayer::create();
        gameLayer->setDelegator(statusLayer);
        this->addChild(statusLayer);
        
        auto optionLayer = OptionLayer::create();
        optionLayer->setDelegator(gameLayer);
        this->addChild(optionLayer);
        
        return true;
    } else {
        return false;
    }
}

void GameScene::restart() {
    this->removeAllChildrenWithCleanup(true);
    this->init();
}
