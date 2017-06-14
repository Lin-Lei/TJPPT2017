#include"DoubleBattle.h"
#include"Vector"

USING_NS_CC;
using namespace CocosDenshion;

extern bool musicSet;
extern bool first;
extern bool soundSet;

Scene* DoubleBattle::createScene() {
	auto scene = Scene::create();
	auto layer = DoubleBattle::create();
	scene->addChild(layer);
	return scene;
}


bool DoubleBattle::init() {
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//创建外边款
	auto doubleBattleScene = Sprite::create("background/_battle.jpg");
	doubleBattleScene->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(doubleBattleScene, 0);

	//创建个人训练地图
	doubleBattleMap = TMXTiledMap::create("map/onetrainmap.tmx");
	doubleBattleMap->setAnchorPoint(Vec2(0.5, 0.5));
	doubleBattleMap->setPosition(Vec2(origin.x + visibleSize.width / 2 - 80, origin.y + visibleSize.height / 2));
	addChild(doubleBattleMap, 1);

	building = doubleBattleMap->getLayer("building");

	//返回图片菜单
	auto returnItem = MenuItemImage::create(
		"button/backnormal.jpg",
		"button/backselect.jpg",
		CC_CALLBACK_1(DoubleBattle::menuReturnCallback, this));
	returnItem->setPosition(Vec2(origin.x + visibleSize.width - 85,
		origin.y + 25));
	auto menu = Menu::create(returnItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	//创建人物
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero/hero1Basic.plist");//缓存人物动画
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Bubble/bubble_all.plist");//缓存泡泡动画
	bubble = Bubble::create("bubble0.png");
	bubble->setAnchorPoint(Vec2(0.5f, 0.5f));
	bubble->setPosition(Vec2(origin.x, origin.y));
	this->addChild(bubble, 9);

	hero = Hero::create("hero1Down.png", 1);
	hero->setScene(building, doubleBattleMap);
	bubble->setScene(building, doubleBattleMap);
	hero->setPosition(Vec2(origin.x + visibleSize.width / 2 - 75, origin.y + visibleSize.height / 2));
	this->addChild(hero, 10, HERO_1);

	hero2 = Hero::create("hero1Down.png", 2);
	hero2->setScene(building, doubleBattleMap);
	hero2->setPosition(Vec2(origin.x + visibleSize.width / 2 - 75, origin.y + visibleSize.height / 2));
	this->addChild(hero2, 10, HERO_2);



	//初始化Map容器
	keyCodeMap1 = std::map<cocos2d::EventKeyboard::KeyCode, bool>();
	resetKeyCodeMap1();
	keyCodeMap2 = std::map<cocos2d::EventKeyboard::KeyCode, bool>();
	resetKeyCodeMap2();

	pressCnt1 = 0; //优化多按键时的判定
	validPress1 = false;

	pressCnt2 = 0; //优化多按键时的判定
	validPress2 = false;

	//注册 键盘事件监听器
	heroKeyboardListener = EventListenerKeyboard::create();

	//按下时调用
	heroKeyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
			bubble->placeBubble(hero->getPosition(), hero);//完成放泡泡功能
		}
		if (hero->getAnimationPlaying())//正在做的动画停止
		{
			hero->stopAllActions();
			hero->setAnimationPlaying(false);
		}

		if (checkArrow(keyCode))
		{
			validPress1 = true;
			pressCnt1++;
			onPressCode1 = keyCode;
			keyCodeMap1[keyCode] = true;
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
			bubble->placeBubble(hero2->getPosition(), hero2);//完成放泡泡功能
		}
		if (hero2->getAnimationPlaying())//正在做的动画停止
		{
			hero2->stopAllActions();
			hero2->setAnimationPlaying(false);
		}

		if (checkP2(keyCode))
		{
			validPress2 = true;
			pressCnt2++;
			onPressCode2 = keyCode;
			keyCodeMap2[keyCode] = true;
		}
	};

	//松开时调用
	heroKeyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (checkArrow(keyCode))
		{
			pressCnt1--;
			keyCodeMap1[keyCode] = false;
			if (pressCnt1 == 1)
			{
				validPress1 = true;
				hero->stopAllActions();
				hero->setAnimationPlaying(false);
				std::map<cocos2d::EventKeyboard::KeyCode, bool>::iterator it;
				for (it = keyCodeMap1.begin(); it != keyCodeMap1.end(); it++)
				{
					if (it->second)
					{
						onPressCode1 = it->first;
					}
				}
			}
			else
			{
				validPress1 = false;
				if (hero->getAnimationPlaying())
				{
					hero->stopAllActions();
					hero->setAnimationPlaying(false);
				}
				hero->setFrame(keyCode);
			}
		}

		if (checkP2(keyCode))
		{
			pressCnt2--;
			keyCodeMap2[keyCode] = false;
			if (pressCnt2 == 1)
			{
				validPress2 = true;
				hero2->stopAllActions();
				hero2->setAnimationPlaying(false);
				std::map<cocos2d::EventKeyboard::KeyCode, bool>::iterator it;
				for (it = keyCodeMap2.begin(); it != keyCodeMap2.end(); it++)
				{
					if (it->second)
					{
						onPressCode2 = it->first;
					}
				}
			}
			else
			{
				validPress2 = false;
				if (hero2->getAnimationPlaying())
				{
					hero2->stopAllActions();
					hero2->setAnimationPlaying(false);
				}
				hero2->setFrame(keyCode);
			}
		}


	};

	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(heroKeyboardListener, this);

	this->scheduleUpdate();

	return true;
}

//进入游戏场景
void DoubleBattle::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();
	if (musicSet) SimpleAudioEngine::getInstance()->playBackgroundMusic("music/gamestartmusic.mp3", true);
	first = true;
}

//清除场景
void DoubleBattle::cleanup() {
	Layer::cleanup();
	if (musicSet) SimpleAudioEngine::getInstance()->stopBackgroundMusic("music/gamestartmusic.mp3");
}

//返回主界面
void DoubleBattle::menuReturnCallback(cocos2d::Ref* pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	Director::getInstance()->popScene();
}

void DoubleBattle::update(float dt)//每秒60次更新
{

	if (validPress1)
	{
		hero->moveHero(onPressCode1);
		hero->setAnimationPlaying(true);
	}

	if (validPress2)
	{
		hero2->moveHero(onPressCode2);
		hero2->setAnimationPlaying(true);
	}

}

void DoubleBattle::resetKeyCodeMap1()
{
	keyCodeMap1[EventKeyboard::KeyCode::KEY_LEFT_ARROW] = false;
	keyCodeMap1[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = false;
	keyCodeMap1[EventKeyboard::KeyCode::KEY_DOWN_ARROW] = false;
	keyCodeMap1[EventKeyboard::KeyCode::KEY_UP_ARROW] = false;

}

void DoubleBattle::resetKeyCodeMap2()
{
	keyCodeMap2[EventKeyboard::KeyCode::KEY_A] = false;
	keyCodeMap2[EventKeyboard::KeyCode::KEY_D] = false;
	keyCodeMap2[EventKeyboard::KeyCode::KEY_S] = false;
	keyCodeMap2[EventKeyboard::KeyCode::KEY_W] = false;

}

bool DoubleBattle::checkArrow(EventKeyboard::KeyCode keyCode)//检测P1按键
{
	return (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW
		|| keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW
		|| keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW
		|| keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW);
}

bool DoubleBattle::checkP2(EventKeyboard::KeyCode keyCode)//检测P2按键
{
	return (keyCode == EventKeyboard::KeyCode::KEY_A
		|| keyCode == EventKeyboard::KeyCode::KEY_D
		|| keyCode == EventKeyboard::KeyCode::KEY_W
		|| keyCode == EventKeyboard::KeyCode::KEY_S);
}

