//
//  AtlasLoader.cpp
//  hellWorld
//
//  Created by gao.da on 1/21/15.
//
//
#include <sstream>
#include <iostream>
#include "AtlasLoader.h"
USING_NS_CC;
using namespace std;

AtlasLoader* AtlasLoader::sharedAtlasLoader = nullptr;

AtlasLoader* AtlasLoader::getInstance() {
    if (sharedAtlasLoader == nullptr) {
        sharedAtlasLoader = new AtlasLoader();
        if (!sharedAtlasLoader->init()) {
            delete sharedAtlasLoader;
            sharedAtlasLoader = nullptr;
            CCLOG("Failed to init atlasloader!");
        }
    }
    return sharedAtlasLoader;
}

void AtlasLoader::destoryInstance() {
    CC_SAFE_DELETE(sharedAtlasLoader);
}

AtlasLoader::AtlasLoader(){}

bool AtlasLoader::init(){
    return true;
}

void AtlasLoader::loadAtlas(string filename){
    auto textureAtlas = Director::getInstance()->getTextureCache()->addImage("atlas.png");
    this->loadAtlas(filename, textureAtlas);
}

void AtlasLoader::loadAtlas(string filename, Texture2D* texture2D) {
    string data = FileUtils::getInstance()->getStringFromFile(filename);
    istringstream stream1(data);
    Atlas atlas;
    
    while(!stream1.eof()) {
        string line;
        getline(stream1, line);
        CCLOG("%s", line.c_str());
        
        istringstream stream2(line);
        stream2 >> atlas.name >> atlas.width >> atlas.height >> atlas.start.x >> atlas.start.y
        >> atlas.end.x >> atlas.end.y;
        atlas.start.x *= 1024;
        atlas.start.y *= 1024;
        atlas.end.x *= 1024;
        atlas.end.y *= 1024;
        
        //CCLOG("%s %d %d %f %f", atlas.name, atlas.width, atlas.height, atlas.start.x, atlas.start.y);
        Rect rect = Rect(atlas.start.x, atlas.start.y, atlas.width, atlas.height);
        auto frame = SpriteFrame::createWithTexture(texture2D, rect);
     
        string key = string(atlas.name);
        //this->_spriteFrames.insert({key, frame});
        this->_spriteFrames.insert(key, frame);
    }
}

SpriteFrame* AtlasLoader::getSpriteFrameByName(string name){
    return this->_spriteFrames.at(name);
}
