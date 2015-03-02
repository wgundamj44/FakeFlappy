//
//  OptionLayer.cpp
//  hellWorld
//
//  Created by gao.da on 1/23/15.
//
//

#include "OptionLayer.h"
USING_NS_CC;
using namespace std;

OptionLayer::OptionLayer() {};
OptionLayer::~OptionLayer() {};

bool OptionLayer::init() {
    if(!Layer::init()) {
        return false;
    }
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    
    listener->onTouchesBegan = CC_CALLBACK_2(OptionLayer::onTouchBegin, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    return true;
}

void OptionLayer::onTouchBegin(vector<Touch*>, Event *event) {
    this->delegator->onTouch();
}


