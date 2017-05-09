#include"OneTrain.h"

USING_NS_CC;

Scene* OneTrain::createScene(){
	auto scene = Scene::create();
	auto layer = OneTrain::create();
	scene->addChild(layer);
	return scene;
}


bool OneTrain::init(){
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//������߿�
	auto oneTrainScene = Sprite::create("_battle.jpg");
	oneTrainScene->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(oneTrainScene,0);

	//��������ѵ����ͼ
	oneTrainMap = TMXTiledMap::create("map/onetrainmap.tmx");
	oneTrainMap->setAnchorPoint(Vec2(0.5, 0.5));
	oneTrainMap->setPosition(Vec2(origin.x + visibleSize.width / 2-70, origin.y + visibleSize.height / 2));
	addChild(oneTrainMap, 1);
	
	//�˳�ͼƬ�˵�
	auto closeItem = MenuItemImage::create(
		"backnormal.jpg",
		"backselect.jpg",
		CC_CALLBACK_1(OneTrain::menuReturnCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width -150,
		origin.y + 80));
	auto menu = Menu::create (closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	return true;
}

void OneTrain::menuReturnCallback(cocos2d::Ref* pSender) {
	Director::getInstance()->popScene();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

}