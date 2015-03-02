//
//  WelcomeLayer.h
//  hellWorld
//
//  Created by gao.da on 1/22/15.
//
//

#ifndef hellWorld_WelcomeLayer_h
#define hellWorld_WelcomeLayer_h
#pragma once
#include "cocos2d.h"
#include "BirdSprite.h"
#include "extensions/cocos-ext.h"
//#include "HttpClient.h"
#include "network/HttpClient.h"

const int START_BUTTON_TAG = 100;

class WelcomeLayer: public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate  {
public:
    WelcomeLayer();
    ~WelcomeLayer();
    virtual bool init();
    CREATE_FUNC(WelcomeLayer);
    
private:
    void menuStartCallback(cocos2d::Ref* sender);
    
    void scrollLand(float dt);
    
    cocos2d::Sprite* land1;
    cocos2d::Sprite* land2;
    BirdSprite *bird;
    
    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::extension::EditBox *editBox;
    cocos2d::Label *loadingLabel;
    
    virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);
    void loginCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void displayMenu();
};


#endif
