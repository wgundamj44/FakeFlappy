//
//  GameLayer.cpp
//  hellWorld
//
//  Created by gao.da on 1/23/15.
//
//

#include "GameLayer.h"
#include "BackgroundLayer.h"
#include "AtlasLoader.h"
USING_NS_CC;

GameLayer::GameLayer() {}
GameLayer::~GameLayer() {}

bool GameLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    
    
    Size visiableSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    this->gameStatus = GAME_STATUS_READY;
    this->score = 0;
    
    this->bird = BirdSprite::getInstance();
    this->bird->createBird();
    PhysicsBody *body = PhysicsBody::create();
    body->addShape(PhysicsShapeCircle::create(BirdSprite::BIRD_RAIDUS));
    body->setDynamic(true);
    body->setLinearDamping(0.0f);
    body->setGravityEnable(true);
    body->setContactTestBitmask(0x01);
    this->bird->setPhysicsBody(body);
    this->bird->setPosition(origin.x + visiableSize.width * 1 / 3 - 5, origin.y + visiableSize.height / 2 + 5);
    CCLOG("init scene %f %f", this->bird->getPositionX(), this->bird->getPositionY());
    this->addChild(bird);
    
    this->groundNode = Node::create();
    float landHeight = BackgroundLayer::getLandHeight();
    auto groundBody  = PhysicsBody::create();
    groundBody->addShape(PhysicsShapeBox::create(Size(288, landHeight)));
    groundBody->setDynamic(false);
    groundBody->setLinearDamping(0.0f);
    groundBody->setContactTestBitmask(0x01);
    this->groundNode->setPhysicsBody(groundBody);
    this->groundNode->setPosition(144, landHeight / 2);
    this->addChild(groundNode);
    
    // init land
    this->landSprite1 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
    this->landSprite1->setAnchorPoint(Point::ZERO);
    this->landSprite1->setPosition(Point::ZERO);
    this->addChild(this->landSprite1, 30);
    
    this->landSprite2 = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("land"));
    this->landSprite2->setAnchorPoint(Point::ZERO);
    this->landSprite2->setPosition(this->landSprite1->getContentSize().width-2.0f,0);
    this->addChild(this->landSprite2, 30);
    
    shiftLand = schedule_selector(GameLayer::scrollLand);
    this->schedule(shiftLand, 0.01f);
    this->scheduleUpdate();
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    return true;
}

bool GameLayer::onContactBegin(PhysicsContact& contact) {
    gameOver();
    return true;
}

void GameLayer::onTouch() {
    if (this->gameStatus == GAME_STATUS_OVER) {
        return;
    }
    
    if (this->gameStatus == GAME_STATUS_READY) {
        this->bird->fly();
        this->gameStatus = GAME_STATUS_START;
        this->createPipes();
    } else if(this->gameStatus == GAME_STATUS_START) {
        this->bird->getPhysicsBody()->setVelocity(Vect(0, 260));
    }
}

void GameLayer::update(float dt) {
    if (this->gameStatus == GAME_STATUS_START) {
        this->checkHit();
    }
}

void GameLayer::scrollLand(float dt) {
    this->landSprite1->setPositionX(this->landSprite1->getPositionX() - 2.0f);
	this->landSprite2->setPositionX(this->landSprite1->getPositionX() + this->landSprite1->getContentSize().width - 2.0f);
	if(this->landSprite2->getPositionX() == 0) {
		this->landSprite1->setPositionX(0);
	}

    Size visibleSize = Director::getInstance()->getVisibleSize();
    for(auto singlePipe: pipes) {
        singlePipe->setPositionX(singlePipe->getPositionX() - 2);
        if (singlePipe->getPositionX() < - PIPE_WIDTH) {
            singlePipe->setTag(NEW_PIPE_TAG);
            singlePipe->setPositionX(visibleSize.width);
            singlePipe->setPositionY(getRandomHeight());
            this->score++;
            this->delegator->onGamePlaying(this->score);
        }
    }
}

void GameLayer::createPipes() {
    for (int i = 0; i < VISIBLE_PIPE_COUNT; i++) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Sprite *pipeUp = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_up"));
        Sprite *pipeDown = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("pipe_down"));
        Node *singlePipe = Node::create();
        
        pipeDown->setPosition(0, PIPE_HEIGHT + PIPE_DISTANCE);
        singlePipe->addChild(pipeDown, 0, DOWN_PIPE_TAG);
        singlePipe->addChild(pipeUp, 0, UP_PIPE_TAG);
        singlePipe->setPosition(visibleSize.width + i * PIPE_INTERVAL + WAIT_DISTANCE, getRandomHeight());
        auto body = PhysicsBody::create();
        auto shapeBoxDown = PhysicsShapeBox::create(pipeDown->getContentSize(), PHYSICSSHAPE_MATERIAL_DEFAULT, Point(0, PIPE_HEIGHT + PIPE_DISTANCE));
        body->addShape(shapeBoxDown);
        body->addShape(PhysicsShapeBox::create(pipeUp->getContentSize()));
        body->setDynamic(false);
        body->setContactTestBitmask(0x01);
        singlePipe->setPhysicsBody(body);
        singlePipe->setTag(NEW_PIPE_TAG);
        
        this->addChild(singlePipe);
        pipes.push_back(singlePipe);
    }
}

void GameLayer::checkHit() {
    for (auto pipe: this->pipes) {
        if (pipe->getTag() == NEW_PIPE_TAG) {
            if(pipe->getPositionX() < this->bird->getPositionX()) {
                pipe->setTag(PASS_PIPE_TAG);
                CCLOG("PASS");
            }
        }
    }
}

int GameLayer::getRandomHeight() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return rand()%(int)(2*PIPE_HEIGHT + PIPE_DISTANCE - visibleSize.height);
}

void GameLayer::gameOver() {
    if(this->gameStatus == GAME_STATUS_OVER) {
        return;
    }
    this->unschedule(shiftLand);
    this->bird->die();
    this->bird->setRotation(-90);
    this->gameStatus = GAME_STATUS_OVER;
    this->birdSpriteFadeout();
    this->delegator->onGameEnd();
}

void GameLayer::birdSpriteFadeout() {
    auto fadeout = FadeOut::create(1.0f);
    auto done = CallFunc::create(CC_CALLBACK_0(GameLayer::birdSpriteRemove, this));
    auto sequence = Sequence::createWithTwoActions(fadeout, done);
    this->bird->stopAllActions();
    this->bird->runAction(sequence);
}

void GameLayer::birdSpriteRemove() {
    this->bird->setRotation(0);
    this->removeChild(this->bird);
}