#ifndef __MAP2_H__
#define __MAP2_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
#include"Bubble.h"

class Map2 : public cocos2d::Layer
{
	Hero *hero;
	Hero *hero2;
	Bubble* bubble;
	cocos2d::EventListenerKeyboard *heroKeyboardListener;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyCodeMap1;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyCodeMap2;
	int pressCnt1, pressCnt2;//优化多按键时的判定，优化体验
	cocos2d::EventKeyboard::KeyCode onPressCode1;
	cocos2d::EventKeyboard::KeyCode onPressCode2;
	bool validPress1, validPress2;

public:
	static cocos2d::Scene* createScene();

	cocos2d::TMXLayer* building;
	cocos2d::TMXLayer* barrierLayer;
	cocos2d::TMXTiledMap* map2;//指向地图的

	virtual bool init();
	virtual void update(float dt);
	virtual void onEnterTransitionDidFinish();
	virtual void cleanup();

	void menuReturnCallback(cocos2d::Ref* pSender);
	void resetKeyCodeMap1();
	void resetKeyCodeMap2();

	bool checkArrow(cocos2d::EventKeyboard::KeyCode keyCode);//检测P1按键
	bool checkP2(cocos2d::EventKeyboard::KeyCode keyCode);//检测P2按键

	CREATE_FUNC(Map2);

};

#endif 
