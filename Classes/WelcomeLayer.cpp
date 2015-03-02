//
//  WelcomeLayer.cpp
//  hellWorld
//
//  Created by gao.da on 1/22/15.
//
//

#include "WelcomeLayer.h"
#include "AtlasLoader.h"
#include "GameScene.h"
#include "picojson.h"
#include "GameData.h"
#include <vector>

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocos2d::network;

WelcomeLayer::WelcomeLayer() {};
WelcomeLayer::~WelcomeLayer() {};

bool WelcomeLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    this->visibleSize = visibleSize;
    this->origin = origin;

    Sprite *backgorund;
    backgorund = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("bg_day"));
    backgorund->setAnchorPoint(Point::ZERO);
    backgorund->setPosition(Point::ZERO);
    this->addChild(backgorund);
    
    Sprite *title = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("title"));
    title->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 /7));
    this->addChild(title);
    
    // Add the land
	this->land1 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
	this->land1->setAnchorPoint(Point::ZERO);
	this->land1->setPosition(Point::ZERO);
	this->addChild(this->land1);
    
	this->land2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
	this->land2->setAnchorPoint(Point::ZERO);
	this->land2->setPosition(this->land1->getContentSize().width - 2.0f, 0);
	this->addChild(this->land2);
    
    this->schedule(schedule_selector(WelcomeLayer::scrollLand), 0.01f);
    
    this->bird = BirdSprite::getInstance();
    this->bird->createBird();
    this->bird->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->bird->setTag(BirdSprite::BIRD_TAG);
    this->bird->idle();
    this->addChild(this->bird);
    
    auto editBox = EditBox::create(Size(240, 50), "white.png");
    editBox->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - editBox->getContentSize().height * 2));
    editBox->setFont("Paint Boy", 32);
    editBox->setFontColor(Color3B::BLACK);
    editBox->setPlaceHolder("Login: ");
    editBox->setReturnType(EditBox::KeyboardReturnType::DONE);
    editBox->setDelegate(this);
    this->addChild(editBox);
    
    this->editBox = editBox;
    return true;
}

void WelcomeLayer::scrollLand(float dt) {
    this->land1->setPositionX(this->land1->getPositionX() - 2.0f);
    this->land2->setPositionX(this->land1->getPositionX() + this->land1->getContentSize().width - 2.0f);
    if (this->land2->getPositionX() == 0) {
        this->land1->setPositionX(0);
    }
}

void WelcomeLayer::menuStartCallback(Ref* sender) {
    //remove bird sprite
    this->removeChildByTag(BirdSprite::BIRD_TAG);
    
    auto gameScene = GameScene::create();
    Director::getInstance()->replaceScene(gameScene);

}

void WelcomeLayer::editBoxReturn(EditBox * editBox) {
    auto request = new HttpRequest();
    request->setUrl("https://fierce-journey-9577.herokuapp.com/newUser");
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(WelcomeLayer::loginCallback, this));
    
    picojson::object object;
    object.insert(make_pair("name", picojson::value(editBox->getText())));
    picojson::value val(object);
    string json = val.serialize();
    request->setRequestData(json.c_str(), json.length());

    auto httpClient = HttpClient::getInstance();
    httpClient->send(request);
    
    this->loadingLabel = Label::createWithTTF("Loading...", "fonts/Marker Felt.ttf", 24);
    auto fadeIn = FadeIn::create(1.0f);
    auto fadeOut = FadeOut::create(1.0f);
    this->loadingLabel->runAction(RepeatForever::create(Sequence::create(fadeOut, fadeIn, nullptr)));
    this->loadingLabel->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 3 / 5));
    this->addChild(loadingLabel);
}

void WelcomeLayer::loginCallback(HttpClient *sender, HttpResponse *response) {
    if (!response->isSucceed()) {
        CCLOG("FAILED");
        return;
    }

    vector<char> *buffer = response->getResponseData();
    string s(buffer->begin(), buffer->end());
    CCLOG("%s", s.c_str());
    picojson::value v;
    string err = picojson::parse(v, s);
    if (!err.empty()) {
        CCLOG("%s", err.c_str());
    } else {
        picojson::object ob = v.get<picojson::object>();
        string &name = ob["userName"].get<string>();
        double    &score = ob["score"].get<double>();
        GameData::highScore = (int)score;
        GameData::userName  = name;
        CCLOG("%s %f", name.c_str(), score);
        this->removeChild(this->editBox);
        this->removeChild(this->loadingLabel);
        char buffer[100];
        sprintf(buffer, "Welcome %s, current score is: %d", GameData::userName.c_str(), GameData::highScore);
        string message = buffer;
        auto scoreLabel = Label::createWithTTF(message, "fonts/Marker Felt.ttf", 24);
        scoreLabel->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 3 / 5));
        this->addChild(scoreLabel);
        displayMenu();
    }
}

void WelcomeLayer::displayMenu() {
    Sprite *startButton = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
    Sprite *activeStartButton = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
    activeStartButton->setPositionY(5);
    
    auto menuItem = MenuItemSprite::create(startButton, activeStartButton, CC_CALLBACK_1(WelcomeLayer::menuStartCallback, this));
    menuItem->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 /5));
    
    auto menu = Menu::create(menuItem, NULL);
    menu->setPosition(Point(origin.x, origin.y));
    menu->setOpacityModifyRGB(0);
    auto fadeIn = FadeIn::create(2.0f);
    this->addChild(menu);
    menu->runAction(fadeIn);

}
