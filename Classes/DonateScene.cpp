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
	// ��ʼ��
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//����Donate�ı���ͼ
	Sprite *donateBackGround = Sprite::create("background/donate.jpg");

	donateBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(donateBackGround);


	//�����ĸ�֧��������ͼƬ
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

	//��һ�����ذ�ť��ȥ
	auto returnMenuItem = MenuItemImage::create("button/oknormal.png",
		"button/okselect.png",
		CC_CALLBACK_1(Donate::menuOkCallBack, this));
	returnMenuItem->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 5);
	Menu *mn = Menu::create(returnMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn,3);

	return true;
}

//������һ����
void Donate::menuOkCallBack(Ref * pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	Director::getInstance()->popScene();
}