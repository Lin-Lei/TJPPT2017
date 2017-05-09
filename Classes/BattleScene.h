#ifndef __Battle_SCENE_H__
#define __Battle_SCENE_H__
#include "cocos2d.h"
#include"OneTrain.h"

class Battle : public cocos2d::Layer{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void Battle::menuReturnCallBack(Ref * pSender);
	void Battle::menuOneTrainCallBack(Ref * pSender);

	CREATE_FUNC(Battle);
};

#endif 
