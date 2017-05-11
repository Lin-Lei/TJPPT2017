#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include"SettingScene.h"
#include"BattleScene.h"
#include"DonateScene.h"
#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void cleanup();

    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuSettingCallBack(cocos2d::Ref* pSender);
	void menuStartCallBack(cocos2d::Ref* pSender);
	void menuDonateCallBack(cocos2d::Ref* psender);

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
