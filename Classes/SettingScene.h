#ifndef __Setting_SCENE_H__
#define __Setting_SCENE_H__
#include "cocos2d.h"
#include"SimpleAudioEngine.h"

class Setting : public cocos2d::Layer
{
	bool isEffect;
public:
	virtual bool init();

	static cocos2d::Scene* createScene();
	void Setting::menuOkCallBack(Ref * pSender);
	void Setting::menuMusicToggleCallBackOn(cocos2d::Ref* psender);
	void Setting::menuMusicToggleCallBackOff(cocos2d::Ref* psender);
	void Setting::menuSoundToggleCallBackOn(cocos2d::Ref* psender);
	void Setting::menuSoundToggleCallBackOff(cocos2d::Ref* psender);

	CREATE_FUNC(Setting);
};

#endif 

