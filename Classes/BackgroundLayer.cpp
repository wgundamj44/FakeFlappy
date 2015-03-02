//
//  BackgroundLayer.cpp
//  hellWorld
//
//  Created by gao.da on 1/22/15.
//
//

#include "BackgroundLayer.h"
#include "AtlasLoader.h"
USING_NS_CC;

BackgroundLayer::BackgroundLayer() {};
BackgroundLayer::~BackgroundLayer() {};

bool BackgroundLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    
    Sprite* background = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("bg_day"));
    
    background->setAnchorPoint(Point::ZERO);
    background->setPosition(Point::ZERO);
    this->addChild(background);

    return true;
}

float BackgroundLayer::getLandHeight() {
    return Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"))->getContentSize().height;
}
