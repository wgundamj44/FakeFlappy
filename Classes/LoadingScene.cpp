/*
 * LoadingScene.cpp
 *
 *  Created on: Jan 20, 2015
 *      Author: gao.da
 */

#include "LoadingScene.h"
#include "AtlasLoader.h"
#include "WelcomeLayer.h"
USING_NS_CC;


bool LoadingScene::init() {
	if (Scene::init()) {
        CCLOG("%s", "init");
        Sprite *background = Sprite::create("splash.png");
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Point origin = Director::getInstance()->getVisibleOrigin();
        background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
        this->addChild(background);
        
        return true;
	} else {
		return false;
	}
}

void LoadingScene::onEnter() {
    Scene::onEnter();
    CCLOG("%s", "on enter");
    Director::getInstance()->getTextureCache()->addImageAsync("atlas.png", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
}

void LoadingScene::loadingCallBack(Texture2D* texture) {
    AtlasLoader::getInstance()->loadAtlas("atlas.txt", texture);
    CCLOG("%s", "after loading");
    Scene *welcomeScene = Scene::create();
    auto welcomeLayer = WelcomeLayer::create();
    welcomeScene->addChild(welcomeLayer);
    
    std::chrono::seconds duration(1);
    std::this_thread::sleep_for( duration );
    Director::getInstance()->replaceScene(welcomeScene);
}





