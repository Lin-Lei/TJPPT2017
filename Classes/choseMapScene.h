#ifndef __CHOSEMAPSCENE_SCENE_H__
#define __CHOSEMAPSCENE_SCENE_H__

#include "cocos2d.h"
#include"map1.h"
#include"map2.h"
#include"map3.h"
#include"SimpleAudioEngine.h"

class ChoseMap : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void map1CallBack(cocos2d::Ref* psender);
	void map2CallBack(cocos2d::Ref* psender);
	void map3CallBack(cocos2d::Ref* psender);

	CREATE_FUNC(ChoseMap);
};

#endif 
