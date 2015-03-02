//
//  AtlasLoader.h
//  hellWorld
//
//  Created by gao.da on 1/21/15.
//
//

#ifndef hellWorld_AtlasLoader_h
#define hellWorld_AtlasLoader_h
#pragma once
#include "cocos2d.h"
#include <unordered_map>

typedef struct _atlas {
    char name[255];
    int width;
    int height;
    cocos2d::Point start;
    cocos2d::Point end;
} Atlas;

class AtlasLoader {
public:
    static AtlasLoader* getInstance();
    static void destoryInstance();
    
    void loadAtlas(std::string filename);
    void loadAtlas(std::string filename, cocos2d::Texture2D* texture2D);
    
    cocos2d::SpriteFrame *getSpriteFrameByName(std::string name);
protected:
    AtlasLoader();
    virtual bool init();
    static AtlasLoader* sharedAtlasLoader;
    
    //std::unordered_map<std::string, cocos2d::SpriteFrame*> _spriteFrames;
    cocos2d::Map<std::string, cocos2d::SpriteFrame*> _spriteFrames;
};


#endif
