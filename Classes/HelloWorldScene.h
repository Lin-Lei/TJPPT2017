#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include"SettingScene.h"
#include"BattleScene.h"
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuSettingCallBack(cocos2d::Ref* pSender);
	void menuStartCallBack(cocos2d::Ref* pSender);

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
