#ifndef __TWOPLAYERSEXPLAIN_SCENE_H__
#define __TWOPLAYERSEXPLAIN_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
#include"choseMapScene.h"

class TwoExplain : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuBeginCallBack(cocos2d::Ref* psender);

	CREATE_FUNC(TwoExplain);
};

#endif 
