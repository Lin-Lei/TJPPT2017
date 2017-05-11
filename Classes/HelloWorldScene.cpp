#include "HelloWorldScene.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool first;
extern bool musicSet;
extern bool soundSet;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}


bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


	// 游戏开始界面
	auto beginScene = Sprite::create("background/begin.jpg");
	beginScene->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(beginScene);


	//设置图片菜单
	auto settingMenuItem = MenuItemImage::create(
		"button/settingnormal.jpg",
		"button/settingselect.jpg",
		CC_CALLBACK_1(HelloWorld::menuSettingCallBack, this));
	settingMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + 250));


	//开始游戏图片菜单
	auto startMenuItem = MenuItemImage::create(
		"button/startnormal.jpg",
		"button/startselect.jpg",
		CC_CALLBACK_1(HelloWorld::menuStartCallBack, this));
	startMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + 300));

    // 退出游戏图片菜单
    auto closeItem = MenuItemImage::create(
        "button/closenormal.jpg",
        "button/closeselect.jpg",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)); 
    closeItem->setPosition(Vec2(origin.x + visibleSize.width/2 ,
        origin.y + 150));

	//捐助图片菜单
	auto donateItem = MenuItemImage::create(
		"button/donatenormal.jpg",
		"button/donateselect.jpg",
		CC_CALLBACK_1(HelloWorld::menuDonateCallBack, this));
	donateItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + 200));

    auto menu = Menu::create(settingMenuItem,closeItem,startMenuItem,donateItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,1);

    return true;
}

//进入完成后加载函数
void HelloWorld::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();
	if(first&&musicSet) SimpleAudioEngine::getInstance()->playBackgroundMusic("music/backgroundmusic.mp3");
	first = false;
}

//清除场景
void HelloWorld::cleanup() {
	Layer::cleanup();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("music/backgroundmusic.mp3");
}

//退出游戏
void HelloWorld::menuCloseCallback(Ref* pSender)
{
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}

//切换至setting场景
void HelloWorld::menuSettingCallBack(Ref *pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto settingScene = Setting::createScene();
	Director::getInstance()->pushScene(settingScene);
}

//切换至battle场景
void HelloWorld::menuStartCallBack(Ref *pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto battleScene = Battle::createScene();
	Director::getInstance()->pushScene(battleScene);
}

//切换至donate场景
void HelloWorld::menuDonateCallBack(Ref* psender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto donateScene = Donate::createScene();
	Director::getInstance()->pushScene(donateScene);
}