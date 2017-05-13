#include"OneTrain.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool musicSet;
extern bool first;
extern bool soundSet;

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

	//创建外边款
	auto oneTrainScene = Sprite::create("background/_battle.jpg");
	oneTrainScene->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(oneTrainScene,0);

	//创建个人训练地图
	oneTrainMap = TMXTiledMap::create("map/onetrainmap.tmx");
	oneTrainMap->setAnchorPoint(Vec2(0.5, 0.5));
	oneTrainMap->setPosition(Vec2(origin.x + visibleSize.width / 2-75, origin.y + visibleSize.height / 2));
	addChild(oneTrainMap, 1);
	
	//退出图片菜单
	auto closeItem = MenuItemImage::create(
		"button/backnormal.jpg",
		"button/backselect.jpg",
		CC_CALLBACK_1(OneTrain::menuReturnCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width -175,
		origin.y + 45));
	auto menu = Menu::create (closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	//创建人物
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero/hero1Basic.plist");
	hero = Hero::create("hero1Down.png");
	hero->setPosition(Vec2(origin.x + visibleSize.width / 2 - 75, origin.y + visibleSize.height / 2));
	this->addChild(hero, 10, HERO_1);

	//初始化Map容器
	keyCodeMap = std::map<cocos2d::EventKeyboard::KeyCode, bool>();
	resetKeyCodeMap();
	
	pressCnt = 0; //优化多按键时的判定
	validPress = false;

	//注册 键盘事件监听器
	heroKeyboardListener = EventListenerKeyboard::create();
	

	heroKeyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (hero->getAnimationPlaying())
		{
			hero->stopAllActions();
			hero->setAnimationPlaying(false);
		}
		
		if (checkArrow(keyCode))
		{
			validPress = true;
			pressCnt++;
			onPressCode = keyCode;
			keyCodeMap[keyCode] = true;
		}
	};

	heroKeyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (checkArrow(keyCode))
		{
			pressCnt--;
			keyCodeMap[keyCode] = false;
			if (pressCnt == 1)
			{
				validPress = true;
				hero->stopAllActions();
				hero->setAnimationPlaying(false);
				std::map<cocos2d::EventKeyboard::KeyCode, bool>::iterator it;
				for (it = keyCodeMap.begin(); it != keyCodeMap.end(); it++)
				{
					if (it->second)
					{
						onPressCode = it->first;
					}
				}
			}
			else
			{
				validPress = false;
				if (hero->getAnimationPlaying())
				{
					hero->stopAllActions();
					hero->setAnimationPlaying(false);
				}
				hero->setFrame(keyCode);
			}
		}
		
		
	};

	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(heroKeyboardListener, this);
	
	this->scheduleUpdate();

	return true;
}

//进入游戏场景
void OneTrain::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();
	if(musicSet) SimpleAudioEngine::getInstance()->playBackgroundMusic("music/gamestartmusic.mp3");
	first = true;
}

//清除场景
void OneTrain::cleanup() {
	Layer::cleanup();
	if(musicSet) SimpleAudioEngine::getInstance()->stopBackgroundMusic("music/gamestartmusic.mp3");
}

//返回主界面
void OneTrain::menuReturnCallback(cocos2d::Ref* pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	Director::getInstance()->popScene();
}

void OneTrain::update(float dt)
{

	if (validPress)
	{
		hero->moveHero(onPressCode);
		hero->setAnimationPlaying(true);
	}
	
}

void OneTrain::resetKeyCodeMap()
{
	keyCodeMap[EventKeyboard::KeyCode::KEY_LEFT_ARROW] = false;
	keyCodeMap[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = false;
	keyCodeMap[EventKeyboard::KeyCode::KEY_DOWN_ARROW] = false;
	keyCodeMap[EventKeyboard::KeyCode::KEY_UP_ARROW] = false;

}

bool OneTrain::checkArrow(EventKeyboard::KeyCode keyCode)
{
	return (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW
		|| keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW
		|| keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW
		|| keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW);
}
