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


	// ��Ϸ��ʼ����
	auto beginScene = Sprite::create("background/begin.jpg");
	beginScene->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(beginScene);


	//����ͼƬ�˵�
	auto settingMenuItem = MenuItemImage::create(
		"button/settingnormal.jpg",
		"button/settingselect.jpg",
		CC_CALLBACK_1(HelloWorld::menuSettingCallBack, this));
	settingMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + 250));


	//��ʼ��ϷͼƬ�˵�
	auto startMenuItem = MenuItemImage::create(
		"button/startnormal.jpg",
		"button/startselect.jpg",
		CC_CALLBACK_1(HelloWorld::menuStartCallBack, this));
	startMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + 300));

    // �˳���ϷͼƬ�˵�
    auto closeItem = MenuItemImage::create(
        "button/closenormal.jpg",
        "button/closeselect.jpg",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)); 
    closeItem->setPosition(Vec2(origin.x + visibleSize.width/2 ,
        origin.y + 150));

	//����ͼƬ�˵�
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

//������ɺ���غ���
void HelloWorld::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();
	if(first&&musicSet) SimpleAudioEngine::getInstance()->playBackgroundMusic("music/backgroundmusic.mp3");
	first = false;
}

//�������
void HelloWorld::cleanup() {
	Layer::cleanup();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("music/backgroundmusic.mp3");
}

//�˳���Ϸ
void HelloWorld::menuCloseCallback(Ref* pSender)
{
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}

//�л���setting����
void HelloWorld::menuSettingCallBack(Ref *pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto settingScene = Setting::createScene();
	Director::getInstance()->pushScene(settingScene);
}

//�л���battle����
void HelloWorld::menuStartCallBack(Ref *pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto battleScene = Battle::createScene();
	Director::getInstance()->pushScene(battleScene);
}

//�л���donate����
void HelloWorld::menuDonateCallBack(Ref* psender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	auto donateScene = Donate::createScene();
	Director::getInstance()->pushScene(donateScene);
}