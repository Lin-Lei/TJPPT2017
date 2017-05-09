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
	// ��ʼ��
	if (!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	

	//����setting�ı���ͼ
	Sprite *settingBackGround = Sprite::create("setting.jpg");

	settingBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(settingBackGround);


	//��������Ч���롰���֡��ı�ǩ
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

	//��Ч�����ֵ�ͼƬ��ӣ�֮�����Ƴɲ˵���
	auto sound_ok = Sprite::create("sound_on1.png");
	auto music_ok = Sprite::create("sound_on2.png");

	sound_ok->setPosition(Vec2(origin.x + visibleSize.width / 2-150,
							   origin.y + visibleSize.height / 2));
	music_ok->setPosition(Vec2(origin.x + visibleSize.width / 2-150,
							   origin.y + visibleSize.height / 2 - 100));
		
	this->addChild(sound_ok,1);
	this->addChild(music_ok,1);

	//��һ�����ذ�ť��ȥ
	auto returnMenuItem = MenuItemImage::create("oknormal.png",
												"okselect.png",
												CC_CALLBACK_1(Setting::menuOkCallBack, this));
	returnMenuItem->setPosition(origin.x + visibleSize.width / 2 - 240, origin.y + visibleSize.height / 2 - 200);
	Menu *mn = Menu::create(returnMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	return true;
}

//������һ����
void Setting::menuOkCallBack(Ref * pSender) {
	Director::getInstance()->popScene();
}