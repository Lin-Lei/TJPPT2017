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
	// ��ʼ��
	if (!Layer::init()) {
		return false;
	}
	finish = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	mid = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	

	text = TextFieldTTF::textFieldWithPlaceHolder("Please input:", "fonts/arial.ttf", 32);
	text->setPosition(Vec2(origin.x + visibleSize.width / 2 - 200, origin.y + visibleSize.height / 2 - 50));
	text->setColor(Color3B::BLACK);
	this->addChild(text, 20);

	recvText = TextFieldTTF::textFieldWithPlaceHolder("", "fonts/arial.ttf", 32)	;
	recvText->setPosition(Vec2(origin.x + visibleSize.width / 2 + 200, origin.y + visibleSize.height / 2 - 50));
	recvText->setColor(Color3B::BLACK);
	this->addChild(recvText, 20);
	recvText->detachWithIME();
	recvText->setString("Message you receive");

	//����RoomScene�ı���ͼ
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

void RoomScene::onEnter() {
	Layer::onEnter();
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event *event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
//			log("enter");
			std::string s = text->getString();
			const char *out = s.c_str();
			send(sockClient, out, strlen(out)+1, 0);
			text->setString("");
			text->detachWithIME();//�رռ�������
			recv(sockClient, recvBuf, 100, 0);
//			log("recieve suceed")
			const std::string m(recvBuf);
			recvText->setString(m);
			text->attachWithIME();//������Ϣ֮�󣬴򿪼�������
		}
	};
	EventDispatcher* eventDsipatcher = Director::getInstance()->getEventDispatcher();
	eventDsipatcher->addEventListenerWithSceneGraphPriority(listener, this);

}


void RoomScene::menuCreatRoomCallBack(Ref* pSender) {//server
	WSADATA wsaData;
	SOCKET sockServer;
	SOCKADDR_IN addrServer;
	SOCKADDR_IN addrClient;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY��ʾ�κ�IP
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(12527);//�󶨶˿�12527
	bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
	listen(sockServer, 1);//1Ϊ�ȴ�������Ŀ
	int len = sizeof(SOCKADDR);

					  //���������̣�ֱ���пͻ�����������Ϊֹ
	sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &len);

	//���ղ���ӡ�ͻ�������
	send(sockClient, "Helloworld", 11, 0);//�����Ϣ
	text->attachWithIME();//��ʼ���Խ�������
	recv(sockClient, recvBuf, 100, 0);
	const std::string m(recvBuf);
	recvText->setString(m);
}

void RoomScene::menuConnectRoomCallBack(Ref* pSender){//client
	WSADATA wsaData;//�ͻ���Socket
	SOCKADDR_IN addrServer;//����˵�ַ

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	addrServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.106");//Ŀ��IP(127.0.0.1�ǻ��͵�ַ)
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(12527);//���Ӷ˿�6000
									  //���ӵ������
	connect(sockClient, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

	//��������
	send(sockClient, "HelloWorld",11, 0);
	for (int i = 1; i <= 2; i++) {//�ͻ����������ν�����Ϣ
		recv(sockClient, recvBuf, 100, 0);
		const std::string m(recvBuf);
		recvText->setString(m);
	}
	text->attachWithIME();
}


void RoomScene::cleanup() {
	closesocket(sockClient);
	WSACleanup();
	Layer::cleanup();
}