//
//  OptionLayer.h
//  hellWorld
//
//  Created by gao.da on 1/23/15.
//
//

#ifndef hellWorld_OptionLayer_h
#define hellWorld_OptionLayer_h
#pragma once
#include "cocos2d.h"
#include <vector>

class OptionDelegate {
public:
    virtual void onTouch() = 0;
};

class OptionLayer: public cocos2d::Layer {
public:
    OptionLayer();
    ~OptionLayer();
    
    virtual bool init();
    CREATE_FUNC(OptionLayer);
    
    void onTouchBegin(const std::vector<cocos2d::Touch*> touch, cocos2d::Event* event);
    
    CC_SYNTHESIZE(OptionDelegate*, delegator, Delegator);
};


#endif
