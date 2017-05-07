#include"SettingScene.h"

USING_NS_CC;

Scene* Setting::createScene()
{
	auto scene = Scene::create();
	auto layer = Setting::create();
	scene->addChild(layer);
	return scene;
}


bool Setting::init()
{
	// 初始化
	if (!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	

	//创建setting的背景图
	Sprite *settingBackGround = Sprite::create("setting.jpg");

	settingBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(settingBackGround);


	//创建“音效”与“音乐”的标签
	auto sound = Label::createWithTTF("sound", "fonts/Marker Felt.ttf", 50);
	sound->setPosition(Vec2(origin.x + visibleSize.width / 2 - 300, 
							origin.y + visibleSize.height/2));
	auto music = Label::createWithTTF("music", "fonts/Marker Felt.ttf", 50);
	music->setPosition(Vec2(origin.x + visibleSize.width / 2 - 300,
							origin.y + visibleSize.height / 2 - 100));
	music->setColor(Color3B::BLACK);
	sound->setColor(Color3B::BLACK);
	this->addChild(sound, 1);
	this->addChild(music, 1);

	//音效、音乐的图片添加（之后完善成菜单）
	auto sound_ok = Sprite::create("sound_on1.png");
	auto music_ok = Sprite::create("sound_on2.png");

	sound_ok->setPosition(Vec2(origin.x + visibleSize.width / 2-150,
							   origin.y + visibleSize.height / 2));
	music_ok->setPosition(Vec2(origin.x + visibleSize.width / 2-150,
							   origin.y + visibleSize.height / 2 - 100));
		
	this->addChild(sound_ok,1);
	this->addChild(music_ok,1);

	//加一个返回按钮上去
	auto returnMenuItem = MenuItemImage::create("oknormal.png",
												"okselect.png",
												CC_CALLBACK_1(Setting::menuOkCallBack, this));
	returnMenuItem->setPosition(origin.x + visibleSize.width / 2 - 240, origin.y + visibleSize.height / 2 - 200);
	Menu *mn = Menu::create(returnMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	return true;
}

//返回上一场景
void Setting::menuOkCallBack(Ref * pSender) {
	Director::getInstance()->popScene();
}