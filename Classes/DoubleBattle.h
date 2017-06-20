#ifndef __Double_BATTLE_H__
#define __Double_BATTLE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
#include"Bubble.h"

class DoubleBattle : public cocos2d::Layer
{
	Hero *hero;
	Hero *hero2;
	Bubble* bubble;
	cocos2d::EventListenerKeyboard *heroKeyboardListener;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyCodeMap1;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyCodeMap2;
	int pressCnt1, pressCnt2;//�Ż��ఴ��ʱ���ж����Ż�����
	cocos2d::EventKeyboard::KeyCode onPressCode1;
	cocos2d::EventKeyboard::KeyCode onPressCode2;
	bool validPress1, validPress2;

public:
	static cocos2d::Scene* createScene();

	cocos2d::TMXLayer* building;
	cocos2d::TMXLayer* barrierLayer;
	cocos2d::TMXTiledMap* doubleBattleMap;//ָ���ͼ��

	virtual bool init();
	virtual void DoubleBattle::update(float dt);
	virtual void DoubleBattle::onEnterTransitionDidFinish();
	virtual void DoubleBattle::cleanup();

	void menuReturnCallback(cocos2d::Ref* pSender);
	void resetKeyCodeMap1();
	void resetKeyCodeMap2();

	bool checkArrow(cocos2d::EventKeyboard::KeyCode keyCode);//���P1����
	bool checkP2(cocos2d::EventKeyboard::KeyCode keyCode);//���P2����

	CREATE_FUNC(DoubleBattle);

};

#endif 
