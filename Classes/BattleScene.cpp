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


	//����battle�ı���ͼ
	Sprite *settingBackGround = Sprite::create("battle.jpg");
	settingBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(settingBackGround);


	//����ѵ��ͼƬ�˵�
	auto oneTrainMenuItem = MenuItemImage::create(
		"onetrainnormal.jpg",
		"onetrainselect.jpg",
		CC_CALLBACK_1(Battle::menuOneTrainCallBack, this));
	oneTrainMenuItem->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	//����ͼƬ�˵�
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

//������һ����
void Battle::menuReturnCallBack(Ref * pSender) {
	Director::getInstance()->popScene();
}

//���뵥��ѵ����ͼ,ע������ĳ�������������Ҫ����
void Battle::menuOneTrainCallBack(Ref* pSender) {
	auto oneTrainScene = OneTrain::createScene();
	Director::getInstance()->pushScene(oneTrainScene);
}