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
	char sendBuf[100];//发送至客户端的字符串
	char recvBuf[100];//接受客户端返回的字符串
	SOCKET sockClient;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Vec2 mid;
	bool finish;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuCreatRoomCallBack(Ref* pSender);
	void menuConnectRoomCallBack(Ref* pSender);
	virtual void onEnter();
	virtual void cleanup();
	cocos2d::TextFieldTTF* text;
	cocos2d::TextFieldTTF* recvText;
	CREATE_FUNC(RoomScene);
};

#endif 
