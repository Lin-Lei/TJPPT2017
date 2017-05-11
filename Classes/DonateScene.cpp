#include"DonateScene.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool soundSet;

Scene* Donate::createScene()
{
	auto scene = Scene::create();
	auto layer = Donate::create();
	scene->addChild(layer);
	return scene;
}


bool Donate::init()
{
	// 初始化
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//创建Donate的背景图
	Sprite *donateBackGround = Sprite::create("background/donate.jpg");

	donateBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(donateBackGround);


	//创建四个支付宝付款图片
	auto one = Sprite::create("donate/one.jpg");
	auto two = Sprite::create("donate/two.jpg");
	auto three = Sprite::create("donate/three.jpg");
	auto four = Sprite::create("donate/four.jpg");

	one->setPosition(Vec2(origin.x + visibleSize.width / 5,
		origin.y + visibleSize.height / 2));
	two->setPosition(Vec2(origin.x + visibleSize.width / 5*2,
		origin.y + visibleSize.height / 2));
	three->setPosition(Vec2(origin.x + visibleSize.width / 5*3,
		origin.y + visibleSize.height / 2));
	four->setPosition(Vec2(origin.x + visibleSize.width / 5*4,
		origin.y + visibleSize.height / 2));
	this->addChild(one, 1);
	this->addChild(two, 1);
	this->addChild(three, 1);
	this->addChild(four, 1);

	//加一个返回按钮上去
	auto returnMenuItem = MenuItemImage::create("button/oknormal.png",
		"button/okselect.png",
		CC_CALLBACK_1(Donate::menuOkCallBack, this));
	returnMenuItem->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 5);
	Menu *mn = Menu::create(returnMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn,3);

	return true;
}

//返回上一场景
void Donate::menuOkCallBack(Ref * pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	Director::getInstance()->popScene();
}