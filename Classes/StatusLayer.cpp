//
//  StatusLayer.cpp
//  hellWorld
//
//  Created by gao.da on 1/27/15.
//
//

#include "StatusLayer.h"
#include "Number.h"
#include "AtlasLoader.h"
#include "GameScene.h"
#include "GameData.h"
#include "picojson.h"
#include <vector>
#include <ostream>
USING_NS_CC;
using namespace cocos2d::network;
using namespace std;


StatusLayer::StatusLayer() {};

StatusLayer::~StatusLayer() {};

bool StatusLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    
    currentScore = 0;
    Number::getInstance()->loadNumber("font", "font_0%2d", 48);
    Number::getInstance()->loadNumber("number_score", "number_score_%02d");
    this->visibleHeight = Director::getInstance()->getVisibleSize().height;
    this->visibleWidth  = Director::getInstance()->getVisibleSize().width;
    this->originPoint   = Director::getInstance()->getVisibleOrigin();
    this->showReadyStatus();
    this->loadWhiteSprite();
    return true;
}

void StatusLayer::showReadyStatus() {
    scoreSprite = (Sprite*) Number::getInstance()->convert("font", 0);
    scoreSprite->setPosition(Point(this->originPoint.x + this->visibleWidth / 2, this->originPoint.y + this->visibleHeight / 2));
    this->addChild(scoreSprite);
}



void StatusLayer::onGameEnd() {
    this->removeChild(scoreSprite);
    this->blinkScreen();
}

void StatusLayer::loadWhiteSprite() {
    whiteSprite = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("score_panel"));
    //whiteSprite->setPosition(this->visibleWidth / 2 + this->originPoint.x,
    //                         this->visibleHeight /2 + this->originPoint.y + 20);
    whiteSprite->setScale(100);
    whiteSprite->setOpacity(0);
    this->addChild(whiteSprite);
}

void StatusLayer::blinkScreen() {
    auto fadeOut = FadeOut::create(0.2f);
    auto fadeIn  = FadeIn::create(0.2f);
    auto blinkAction = Sequence::create(fadeIn, fadeOut, nullptr);
    CallFunc *actionDone = CallFunc::create(CC_CALLBACK_0(StatusLayer::fadeInGameOver, this));
    auto sequence = Sequence::createWithTwoActions(blinkAction, actionDone);
    whiteSprite->stopAllActions();
    whiteSprite->runAction(sequence);
}

void StatusLayer::fadeInGameOver() {
    auto label = Label::createWithTTF("Game Over!", "fonts/Marker Felt.ttf", 32);
    Size size = Director::getInstance()->getVisibleSize();
    label->setPosition(size.width / 2, size.height / 2);
    this->addChild(label);
    this->checkHighScore();
}

void StatusLayer::checkHighScore() {
    if (currentScore >= GameData::highScore) {
        auto request = new HttpRequest();
        request->setUrl("https://fierce-journey-9577.herokuapp.com/setScore");
        request->setRequestType(HttpRequest::Type::POST);
        request->setResponseCallback(CC_CALLBACK_2(StatusLayer::setScoreCallback, this));
        
        picojson::object object;
        object.insert(make_pair("name", picojson::value(GameData::userName)));
        object.insert(make_pair("score", picojson::value((double)currentScore)));
        picojson::value val(object);
        string json = val.serialize();
        CCLOG("json: %s", json.c_str());
        request->setRequestData(json.c_str(), json.length());
        
        auto httpClient = HttpClient::getInstance();
        httpClient->send(request);
    } else {
        this->addRestartBtn();
    }
}

void StatusLayer::setScoreCallback(HttpClient *client, HttpResponse* response) {
    vector<char> *res = response->getResponseData();
    string s(res->begin(), res->end());
    CCLOG("%s", s.c_str());
    picojson::value v;
    string err = picojson::parse(v, s);
    if (!err.empty()) {
        CCLOG("%s", err.c_str());
    } else {
        CCLOG("%s", err.c_str());
        ostringstream os;
        os << "New record: " << currentScore;
        auto highScoreLabel = Label::createWithTTF(os.str(), "fonts/Marker Felt.ttf", 24);
        highScoreLabel->setPosition(Point(originPoint.x + visibleWidth / 2, originPoint.y + visibleHeight * 3 / 4));
        this->addChild(highScoreLabel);
    }
    this->addRestartBtn();

}

void StatusLayer::addRestartBtn() {
    Node *tmpNode = Node::create();
    auto restartBtn = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
    auto restartBtnActive = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
    restartBtnActive->setPositionY(-4);
    auto menuItem = MenuItemSprite::create(restartBtn, restartBtnActive, CC_CALLBACK_1(StatusLayer::menuRestartCallback, this));
    auto menu = Menu::create(menuItem, nullptr);
    menu->setPosition(Point(this->originPoint.x + this->visibleWidth / 2, this->originPoint.y + this->visibleHeight * 2 / 7));

	tmpNode->addChild(menu);
    this->addChild(tmpNode);
    
    auto fadeIn = FadeIn::create(0.1f);
    tmpNode->stopAllActions();
    tmpNode->runAction(fadeIn);
}

void StatusLayer::menuRestartCallback(cocos2d::Ref* pSender) {
    auto scene = GameScene::create();
    //TransitionScene *transition = TransitionFade::create(0.5, scene);
    //Director::getInstance()->replaceScene(transition);
    Director::getInstance()->replaceScene(scene);
}

void StatusLayer::onGamePlaying(int score) {
    this->removeChild(scoreSprite);
    scoreSprite = (Sprite*) Number::getInstance()->convert("font", score);
    scoreSprite->setPosition(Point(this->originPoint.x + this->visibleWidth / 2, this->originPoint.y + this->visibleHeight / 2));
    this->addChild(scoreSprite);
    this->currentScore =  score;

}