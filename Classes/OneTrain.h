#ifndef __One_TRAIN_H__
#define __One_TRAIN_H__

#include "cocos2d.h"
#include"Hero.h"
#include"SimpleAudioEngine.h"

class OneTrain : public cocos2d::Layer
{
	cocos2d::TMXTiledMap* oneTrainMap;
	Hero *hero;
	cocos2d::EventListenerKeyboard *heroKeyboardListener;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyCodeMap;

	int pressCnt;//优化多按键时的判定，优化体验
	cocos2d::EventKeyboard::KeyCode onPressCode;
	bool validPress;
	

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void OneTrain::update(float dt);
	virtual void OneTrain::onEnterTransitionDidFinish();
	virtual void OneTrain::cleanup();

	void menuReturnCallback(cocos2d::Ref* pSender);
	void OneTrain::resetKeyCodeMap();
	bool checkArrow(cocos2d::EventKeyboard::KeyCode keyCode);

	CREATE_FUNC(OneTrain);

	

};

#endif 
