//
//  StatusLayer.h
//  hellWorld
//
//  Created by gao.da on 1/27/15.
//
//

#ifndef hellWorld_StatusLayer_h
#define hellWorld_StatusLayer_h
#pragma once
#include "cocos2d.h"
#include "GameLayer.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"


class StatusLayer : public cocos2d::Layer, public StatusDelegate {
public:
    StatusLayer();
    ~StatusLayer();
    virtual bool init();
    
    CREATE_FUNC(StatusLayer);
    
    void onGameEnd();
    
    void onGamePlaying(int score);
private:
    cocos2d::Sprite* scoreSprite;
    
    void showReadyStatus();
    void fadeInGameOver();
    void blinkScreen();
    void loadWhiteSprite();
    void addRestartBtn();
    void menuRestartCallback(cocos2d::Ref* pSender);
    
    void checkHighScore();
    void setScoreCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    int currentScore;
    int visibleWidth;
    int visibleHeight;
    
    cocos2d::Sprite* whiteSprite;
    cocos2d::Point originPoint;
    static const int CURRENT_SCORE_TAG = 20001;
};


#endif
