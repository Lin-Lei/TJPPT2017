#include"TwoPlayersExplain.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool soundSet;

Scene* TwoExplain::createScene()
{
	auto scene = Scene::create();
	auto layer = TwoExplain::create();
	scene->addChild(layer);
	return scene;
}


bool TwoExplain::init()
{
	// 初始化
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//创建Explain的背景图
	Sprite *onePlayerBackGround = Sprite::create("background/twoPlayers.png");

	onePlayerBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(onePlayerBackGround);

	//加一个按钮上去
	auto beginMenuItem = MenuItemImage::create("button/oknormal.png",
		"button/okselect.png",
		CC_CALLBACK_1(TwoExplain::menuBeginCallBack, this));
	beginMenuItem->setPosition(origin.x + visibleSize.width / 2-50, origin.y + visibleSize.height / 5);
	Menu *mn = Menu::create(beginMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn, 3);

	return true;
}


void TwoExplain::menuBeginCallBack(Ref * pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto choseMapScene = ChoseMap::createScene();
	Director::getInstance()->replaceScene(choseMapScene);
}