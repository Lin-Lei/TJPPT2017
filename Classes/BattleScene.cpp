#include"BattleScene.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool soundSet;

Scene* Battle::createScene(){
	auto scene = Scene::create();
	auto layer = Battle::create();
	scene->addChild(layer);
	return scene;
}


bool Battle::init(){
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//����battle�ı���ͼ
	Sprite *settingBackGround = Sprite::create("background/battle.jpg");
	settingBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(settingBackGround);


	//����ѵ��ͼƬ�˵�
	auto oneTrainMenuItem = MenuItemImage::create(
		"button/onetrainnormal.jpg",
		"button/onetrainselect.jpg",
		CC_CALLBACK_1(Battle::menuOneTrainCallBack, this));
	oneTrainMenuItem->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	//����ͼƬ�˵�
	auto returnMenuItem = MenuItemImage::create(
		"button/backnormal.jpg",
		"button/backselect.jpg",
		CC_CALLBACK_1(Battle::menuReturnCallBack, this));
	returnMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));

	//˫�˶�սͼƬ�˵�
	auto doubleBattleMenuItem = MenuItemImage::create(
		"button/doublenormal.jpg",
		"button/doubleselect.jpg",
		CC_CALLBACK_1(Battle::menuDoubleBattleCallBack, this));
	doubleBattleMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 -50));

	Menu *mn = Menu::create(oneTrainMenuItem,returnMenuItem,doubleBattleMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn,1);


	return true;
}

//������һ����
void Battle::menuReturnCallBack(Ref * pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	Director::getInstance()->popScene();
}

//���뵥��ѵ����ͼ
void Battle::menuOneTrainCallBack(Ref* pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto oneTrainScene = OneExplain::createScene();
	Director::getInstance()->replaceScene(oneTrainScene);
}

//����˫�˶�ս��ͼ
void Battle::menuDoubleBattleCallBack(Ref* pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto doubleBattleScene = TwoExplain::createScene();
	Director::getInstance()->replaceScene(doubleBattleScene);
}