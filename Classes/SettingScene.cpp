#include"SettingScene.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool musicSet;
extern bool soundSet;

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
	Sprite *settingBackGround = Sprite::create("background/setting.jpg");

	settingBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(settingBackGround);


	//创建“音效”与“音乐”的标签
	auto sound = Label::createWithTTF("sound", "fonts/Marker Felt.ttf", 50);
	sound->setPosition(Vec2(origin.x + visibleSize.width / 2 - 300, 
							origin.y + visibleSize.height/2+50));
	auto music = Label::createWithTTF("music", "fonts/Marker Felt.ttf", 50);
	music->setPosition(Vec2(origin.x + visibleSize.width / 2 - 300,
							origin.y + visibleSize.height / 2 - 50));
	music->setColor(Color3B::BLACK);
	sound->setColor(Color3B::BLACK);
	this->addChild(sound, 1);
	this->addChild(music, 1);


	//音乐开关
	auto musicOnMenuItem = MenuItemImage::create(
		"button/music_on.png",
		"button/music_on.png");
	auto musicOffMenuItem = MenuItemImage::create(
		"button/music_off.png",
		"button/music_off.png");
	auto musicToggleMenuItem = MenuItemToggle::createWithCallback(
			CC_CALLBACK_1(Setting::menuMusicToggleCallBackOn, this),
			musicOffMenuItem,
			musicOnMenuItem,
			NULL);
	if (!musicSet) {
			musicToggleMenuItem = MenuItemToggle::createWithCallback(
			CC_CALLBACK_1(Setting::menuMusicToggleCallBackOff, this),
			musicOnMenuItem,
			musicOffMenuItem,
			NULL);
	}
	musicToggleMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - 150,
		origin.y + visibleSize.height / 2 - 50));

	//音效开关
	auto soundOnMenuItem = MenuItemImage::create(
		"button/sound_on.png",
		"button/sound_on.png");
	auto soundOffMenuItem = MenuItemImage::create(
		"button/sound_off.png",
		"button/sound_off.png");
	auto soundToggleMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(Setting::menuSoundToggleCallBackOn, this),
		soundOffMenuItem,
		soundOnMenuItem,
		NULL);
	if (!soundSet) {
		soundToggleMenuItem = MenuItemToggle::createWithCallback(
			CC_CALLBACK_1(Setting::menuSoundToggleCallBackOff, this),
			soundOnMenuItem,
			soundOffMenuItem,
			NULL);
	}

	soundToggleMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2-150,
							   origin.y + visibleSize.height / 2+50));
	
	//加一个返回按钮上去
	auto returnMenuItem = MenuItemImage::create("button/oknormal.png",
												"button/okselect.png",
												CC_CALLBACK_1(Setting::menuOkCallBack, this));
	returnMenuItem->setPosition(origin.x + visibleSize.width / 2 - 240, origin.y + visibleSize.height / 2 - 175);

	Menu *mn = Menu::create(returnMenuItem,musicToggleMenuItem, soundToggleMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	return true;
}

//音乐开关按钮(开启状态
void Setting::menuMusicToggleCallBackOn(Ref* psender) {
	auto musicToggleMenuItem = (MenuItemToggle *)psender;
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	if (musicToggleMenuItem->getSelectedIndex()==1) {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic("music/backgroundmusic.mp3");
	}
	else {
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/backgroundmusic.mp3");
	}
	musicSet = !musicSet;
}
//音乐开关(关闭状态
void Setting::menuMusicToggleCallBackOff(Ref* psender) {
	auto musicToggleMenuItem = (MenuItemToggle *)psender;
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	if (musicToggleMenuItem->getSelectedIndex() == 1) {
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/backgroundmusic.mp3");
	}
	else {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic("music/backgroundmusic.mp3");
	}
	musicSet = !musicSet;
}

//音效开关按钮(开启状态
void Setting::menuSoundToggleCallBackOn(Ref* psender) {
	auto soundToggleMenuItem = (MenuItemToggle *)psender;
	if (soundSet) {
		SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	}
	if (soundToggleMenuItem->getSelectedIndex() == 1) {
		soundSet = false;
	}
	else {
		soundSet = true;
		SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	}
}

//音效开关(关闭状态
void Setting::menuSoundToggleCallBackOff(Ref* psender) {
	auto soundToggleMenuItem = (MenuItemToggle *)psender;
	if (soundSet) {
		SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	}
	if (soundToggleMenuItem->getSelectedIndex() == 1) {
		soundSet = true;
		SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	}
	else {
		soundSet = false;
	}
}
//返回上一场景
void Setting::menuOkCallBack(Ref * pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	Director::getInstance()->popScene();
}