#ifndef __Double_BATTLE_H__
#define __Double_BATTLE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
#include"Bubble.h"

class DoubleBattle : public cocos2d::Layer
{
	Hero *hero;
	Bubble* bubble;
	cocos2d::EventListenerKeyboard *heroKeyboardListener;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyCodeMap;
	int pressCnt;//优化多按键时的判定，优化体验
	cocos2d::EventKeyboard::KeyCode onPressCode;
	bool validPress;

public:
	static cocos2d::Scene* createScene();

	cocos2d::TMXLayer* building;
	cocos2d::TMXTiledMap* doubleBattleMap;//指向地图的

	virtual bool init();
	virtual void DoubleBattle::update(float dt);
	virtual void DoubleBattle::onEnterTransitionDidFinish();
	virtual void DoubleBattle::cleanup();

	void menuReturnCallback(cocos2d::Ref* pSender);
	void resetKeyCodeMap();
	bool checkArrow(cocos2d::EventKeyboard::KeyCode keyCode);

	CREATE_FUNC(DoubleBattle);

};

#endif 
