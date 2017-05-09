#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

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
	auto beginScene = Sprite::create("begin.jpg");
	beginScene->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(beginScene);


	//����ͼƬ�˵�
	auto settingMenuItem = MenuItemImage::create(
		"settingnormal.jpg",
		"settingselect.jpg",
		CC_CALLBACK_1(HelloWorld::menuSettingCallBack, this));
	settingMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + 100));


	//��ʼ��ϷͼƬ�˵�
	auto startMenuItem = MenuItemImage::create(
		"startnormal.jpg",
		"startselect.jpg",
		CC_CALLBACK_1(HelloWorld::menuStartCallBack, this));
	startMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + 150));

    // �˳���ϷͼƬ�˵�
    auto closeItem = MenuItemImage::create(
        "closenormal.jpg",
        "closeselect.jpg",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)); 
    closeItem->setPosition(Vec2(origin.x + visibleSize.width/2 ,
        origin.y + 50));

    auto menu = Menu::create(settingMenuItem,closeItem,startMenuItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,1);

    return true;
}

//�˳���Ϸ
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}

//�л���setting����
void HelloWorld::menuSettingCallBack(Ref *pSender) {
	auto settingScene = Setting::createScene();
	Director::getInstance()->pushScene(settingScene);
}

//�л���battle����
void HelloWorld::menuStartCallBack(Ref *pSender) {
	auto battleScene = Battle::createScene();
	Director::getInstance()->pushScene(battleScene);
}