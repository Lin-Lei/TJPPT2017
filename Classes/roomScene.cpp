#include"roomScene.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool soundSet;

Scene* RoomScene::createScene()
{
	auto scene = Scene::create();
	auto layer = RoomScene::create();
	scene->addChild(layer);
	return scene;
}


bool RoomScene::init()
{
	// 初始化
	if (!Layer::init()) {
		return false;
	}
	
	listener = cocos2d::EventListenerKeyboard::create();

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	//创建RoomScene的背景图
	Sprite *roomBackGround = Sprite::create("background/room.jpg");
	roomBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(roomBackGround,0);

	auto createRoomMenuItem = MenuItemImage::create(
		"button/connectnormal.jpg",
		"button/connectselect.jpg",
		CC_CALLBACK_1(RoomScene::menuCreatRoomCallBack, this));
	createRoomMenuItem->setPosition(origin.x + visibleSize.width / 2-100, origin.y + visibleSize.height / 5 );

	auto connectRoomMenuItem = MenuItemImage::create(
		"button/cnormal.jpg",
		"button/cselect.jpg",
		CC_CALLBACK_1(RoomScene::menuConnectRoomCallBack, this));
	connectRoomMenuItem->setPosition(origin.x + visibleSize.width / 2+100, origin.y + visibleSize.height / 5);


	Menu *mn = Menu::create(connectRoomMenuItem, createRoomMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn, 1);


	return true;
}


void RoomScene::input(char s[]) {
	Sprite* pic;
	pic = Sprite::create("pic/send.png");
	pic->setPosition(Vec2(origin.x + visibleSize.width/8, origin.y + visibleSize.height/6*7));
	this->addChild(pic, 20, 3);
	text->attachWithIME();
	bool out = true;
	while (out) {
		listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode,Event *event){
			if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
				log("enter");
				auto num = text->getCharCount();
				const std::string str = text->getString();
				text->setString("");
				text->detachWithIME();
				for (int i = 0; i < num; i++) s[i] = str[i];
				s[num] = '\0';
				log("%s	%d",s, num);
				out = false;
				this->removeChildByTag(3);
			}
		};
		EventDispatcher* eventDsipatcher = Director::getInstance()->getEventDispatcher();
		eventDsipatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
}


void RoomScene::menuCreatRoomCallBack(Ref* pSender) {//server
	Sprite* waiting;
	waiting = Sprite::create("pic/waiting.png");
	waiting->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 150));
	this->addChild(waiting, 5,9);
	WSADATA wsaData;
	SOCKET sockServer;
	SOCKADDR_IN addrServer;
	SOCKET sockClient;
	SOCKADDR_IN addrClient;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY表示任何IP
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(12527);//绑定端口12527
	bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

	//Listen监听端
	listen(sockServer, 1);//1为等待连接数目
	//printf("服务器已启动:\n监听中...\n");
	int len = sizeof(SOCKADDR);
	char sendBuf[100] = "HelloWorld";//发送至客户端的字符串
	char recvBuf[100];//接受客户端返回的字符串

					  //会阻塞进程，直到有客户端连接上来为止
	sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &len);

	Sprite* rec;
	rec = Sprite::create("pic/recv.png");
	rec->setPosition(Vec2(origin.x + visibleSize.width / 8*7, origin.y + visibleSize.height / 6 * 7));
	this->addChild(rec, 20, 5);

	//文本框实现
	text = TextFieldTTF::textFieldWithPlaceHolder("Please input:", "Arial", 32);
	text->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height / 2 - 100));
	text->setColor(Color3B::BLACK);
	this->addChild(text, 20);

	this->removeChildByTag(9);//移除waiting图像
	//接收并打印客户端数据
	send(sockClient, sendBuf, strlen(sendBuf) + 1, 0);//输出信息
	recv(sockClient, recvBuf, 100, 0);
	const std::string m(recvBuf);
	auto lable=Label::createWithSystemFont(m, "Arial", 32);
	lable->setPosition(Vec2(origin.x + visibleSize.width + 100, origin.y + visibleSize.height / 2 - 100));
	this->addChild(lable, 20,7);
	int result;
	do {
		input(sendBuf);
		send(sockClient, sendBuf, strlen(sendBuf) + 1, 0);
		result = recv(sockClient, recvBuf, 100, 0);
		this->removeChildByTag(7);
		const std::string m(recvBuf);
		auto lable = Label::createWithSystemFont(m, "Arial", 32);
		lable->setPosition(Vec2(origin.x + visibleSize.width + 100, origin.y + visibleSize.height / 2 - 100));
		this->addChild(lable, 20, 7);
	} while (result);
	//关闭socket
	closesocket(sockClient);
	WSACleanup();
}

void RoomScene::menuConnectRoomCallBack(Ref* pSender){//client
	Sprite* waiting;
	waiting = Sprite::create("pic/waiting.png");
	waiting->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 150));
	this->addChild(waiting, 5, 8);
	WSADATA wsaData;
	SOCKET sockClient;//客户端Socket
	SOCKADDR_IN addrServer;//服务端地址
	int result = 0;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//新建客户端socket
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	//定义要连接的服务端地址
	addrServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.106");//目标IP(127.0.0.1是回送地址)
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(12527);//连接端口12527
									   //连接到服务端
	connect(sockClient, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

	Sprite* rec;
	rec = Sprite::create("pic/recv.png");
	rec->setPosition(Vec2(origin.x + visibleSize.width / 8 * 7, origin.y + visibleSize.height / 6 * 7));
	this->addChild(rec, 20, 5);

	//文本框实现
	text = TextFieldTTF::textFieldWithPlaceHolder("Please input:", "Arial", 32);
	text->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height / 2 - 100));
	text->setColor(Color3B::BLACK);
	this->addChild(text, 20);

	this->removeChildByTag(8);


	//发送数据
	char message[100] = "HelloSocket!";
	char recvBuf[100];
	send(sockClient, message, strlen(message) + 1, 0);
	result = recv(sockClient, recvBuf, 100, 0);

	const std::string m(recvBuf);
	auto lable = Label::createWithSystemFont(m, "Arial", 32);
	lable->setPosition(Vec2(origin.x + visibleSize.width + 100, origin.y + visibleSize.height / 2 - 100));
	this->addChild(lable, 20, 7);
	do {
		result = recv(sockClient, recvBuf, 100, 0);
		this->removeChildByTag(7);
		const std::string m(recvBuf);
		auto lable = Label::createWithSystemFont(m, "Arial", 32);
		lable->setPosition(Vec2(origin.x + visibleSize.width + 100, origin.y + visibleSize.height / 2 - 100));
		this->addChild(lable, 20, 7);
		input(message);
		send(sockClient, message, strlen(message) + 1, 0);

	} while (result > 0);
	//关闭socket
	closesocket(sockClient);
	WSACleanup();
}