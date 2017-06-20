#ifndef __ONEPLAYEREXPLAIN_SCENE_H__
#define __ONEPLAYEREXPLAIN_SCENE_H__

#include "cocos2d.h"
#include"OneTrain.h"
#include"SimpleAudioEngine.h"

class OneExplain : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuBeginCallBack(cocos2d::Ref* psender);

	CREATE_FUNC(OneExplain);
};

#endif 
