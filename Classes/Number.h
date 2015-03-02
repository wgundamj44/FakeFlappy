//
//  Number.h
//  hellWorld
//
//  Created by gao.da on 1/27/15.
//
//

#ifndef hellWorld_Number_h
#define hellWorld_Number_h
#pragma once
#include "cocos2d.h"
#include <vector>

enum Gravity {
    GRAVITY_CENTER = 1,
    GRAVITY_LEFT,
    GRAVTIY_RIGHT
};

const int NumberDistance = 4;

class NumberSeries : public cocos2d::Ref {
public:
    NumberSeries();
    ~NumberSeries();
    
    virtual bool init();
    
    CREATE_FUNC(NumberSeries);
    
    void loadNumber(const char* fmt, int base = 0);
    
    cocos2d::SpriteFrame* at(int index);
private:
    cocos2d::Vector<cocos2d::SpriteFrame*> numberSeries;
};

class Number {
public:
    Number();
    ~Number();
    
    static Number* getInstance();
    static void destroyInstance();
    
    bool virtual loadNumber(const char* name, const char* fmt, int base = 0);
    
    cocos2d::Node* convert(const char* name, int number, Gravity gravity = Gravity::GRAVITY_CENTER);
    
    bool virtual init();
    
private:
    cocos2d::Map<std::string, NumberSeries*> numberContainer;
    static Number* sharedNumber;
};


#endif
