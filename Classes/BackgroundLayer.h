//
//  BackgroundLayer.h
//  hellWorld
//
//  Created by gao.da on 1/22/15.
//
//

#ifndef hellWorld_BackgroundLayer_h
#define hellWorld_BackgroundLayer_h
#pragma once
#include "cocos2d.h"

class BackgroundLayer : public cocos2d::Layer {
public:
    BackgroundLayer();
    ~BackgroundLayer();
    
    virtual bool init();
    
    CREATE_FUNC(BackgroundLayer);
    
    static float getLandHeight();
};


#endif
