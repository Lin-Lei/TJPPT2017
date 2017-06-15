#include"OnePlayerExplain.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool soundSet;

Scene* OneExplain::createScene()
{
	auto scene = Scene::create();
	auto layer = OneExplain::create();
	scene->addChild(layer);
	return scene;
}


bool OneExplain::init()
{
	// 初始化
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//创建Explain的背景图
	Sprite *onePlayerBackGround = Sprite::create("background/onePlayer.png");

	onePlayerBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(onePlayerBackGround);

	//加一个按钮上去
	auto beginMenuItem = MenuItemImage::create("button/oknormal.png",
		"button/okselect.png",
		CC_CALLBACK_1(OneExplain::menuBeginCallBack, this));
	beginMenuItem->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 5);
	Menu *mn = Menu::create(beginMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn, 3);

	return true;
}


void OneExplain::menuBeginCallBack(Ref * pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto oneTrainScene = OneTrain::createScene();
	Director::getInstance()->replaceScene(oneTrainScene);
}