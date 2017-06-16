#ifndef __ROOMSCENE_SCENE_H__
#define __ROOMSCENE_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
#include"string"
#include <Winsock2.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")

class RoomScene : public cocos2d::Layer
{

	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	void input(char s[]);
	cocos2d::EventListenerKeyboard* listener;//创建键盘事件监听器
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuCreatRoomCallBack(Ref* pSender);
	void menuConnectRoomCallBack(Ref* pSender);

	cocos2d::TextFieldTTF* text;
	CREATE_FUNC(RoomScene);
};

#endif 
