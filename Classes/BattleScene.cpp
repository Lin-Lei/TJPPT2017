#include"BattleScene.h"

USING_NS_CC;

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


	//创建battle的背景图
	Sprite *settingBackGround = Sprite::create("battle.jpg");
	settingBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(settingBackGround);


	//单人训练图片菜单
	auto oneTrainMenuItem = MenuItemImage::create(
		"onetrainnormal.jpg",
		"onetrainselect.jpg",
		CC_CALLBACK_1(Battle::menuOneTrainCallBack, this));
	oneTrainMenuItem->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	//返回图片菜单
	auto returnMenuItem = MenuItemImage::create(
		"backnormal.jpg",
		"backselect.jpg",
		CC_CALLBACK_1(Battle::menuReturnCallBack, this));
	returnMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));

	Menu *mn = Menu::create(oneTrainMenuItem,returnMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn,1);


	return true;
}

//返回上一场景
void Battle::menuReturnCallBack(Ref * pSender) {
	Director::getInstance()->popScene();
}

//进入单人训练地图,注意最初的场景还被挂起，需要丢掉
void Battle::menuOneTrainCallBack(Ref* pSender) {
	auto oneTrainScene = OneTrain::createScene();
	Director::getInstance()->pushScene(oneTrainScene);
}