#ifndef __One_TRAIN_H__
#define __One_TRAIN_H__
#include "cocos2d.h"

class OneTrain : public cocos2d::Layer{
	cocos2d::TMXTiledMap* oneTrainMap;

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuReturnCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(OneTrain);
};

#endif 
