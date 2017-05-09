#ifndef __Setting_SCENE_H__
#define __Setting_SCENE_H__
#include "cocos2d.h"

class Setting : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	void Setting::menuOkCallBack(Ref * pSender);
	virtual bool init();

	CREATE_FUNC(Setting);
};

#endif 

