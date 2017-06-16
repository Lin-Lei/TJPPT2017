#ifndef __Battle_SCENE_H__
#define __Battle_SCENE_H__
#include "cocos2d.h"
#include"OnePlayerExplain.h"
#include"TwoPlayersExplain.h"
#include"roomScene.h"
#include"SimpleAudioEngine.h"

class Battle : public cocos2d::Layer{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuReturnCallBack(Ref * pSender);
	void menuOneTrainCallBack(Ref * pSender);
	void menuDoubleBattleCallBack(Ref * pSnder);
	void menuOnlineBattleCallBack(Ref* pSender);

	CREATE_FUNC(Battle);
};

#endif 
