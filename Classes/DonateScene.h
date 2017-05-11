#ifndef __DONATE_SCENE_H__
#define __DONATE_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"

class Donate : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuOkCallBack(cocos2d::Ref* psender);

	CREATE_FUNC(Donate);
};

#endif 
