#include"choseMapScene.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool soundSet;

Scene* ChoseMap::createScene()
{
	auto scene = Scene::create();
	auto layer = ChoseMap::create();
	scene->addChild(layer);
	return scene;
}


bool ChoseMap::init()
{
	// 初始化
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//创建ChoseMap的背景图
	Sprite *onePlayerBackGround = Sprite::create("background/chosemap.jpg");

	onePlayerBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(onePlayerBackGround);

	//加选择地图按钮上去
	auto map1MenuItem = MenuItemImage::create(
		"button/map1.jpg",
		"button/map1s.jpg",
		CC_CALLBACK_1(ChoseMap::map1CallBack, this));
	map1MenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 +50));

	auto map2MenuItem = MenuItemImage::create(
		"button/map2.jpg",
		"button/map2s.jpg",
		CC_CALLBACK_1(ChoseMap::map2CallBack, this));
	map2MenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height / 2));

	auto map3MenuItem = MenuItemImage::create(
		"button/map3.jpg",
		"button/map3s.jpg",
		CC_CALLBACK_1(ChoseMap::map3CallBack, this));
	map3MenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height / 2-50));

	Menu *mn = Menu::create(map2MenuItem, map1MenuItem, map3MenuItem, NULL);

	mn->setPosition(Vec2::ZERO);
	this->addChild(mn, 3);

	return true;
}


void ChoseMap::map1CallBack(Ref * pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto map1Scene = Map1::createScene();
	Director::getInstance()->replaceScene(map1Scene);
}

void ChoseMap::map2CallBack(Ref * pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto map2Scene = Map2::createScene();
	Director::getInstance()->replaceScene(map2Scene);
}

void ChoseMap::map3CallBack(Ref * pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto map3Scene = Map3::createScene();
	Director::getInstance()->replaceScene(map3Scene);
}