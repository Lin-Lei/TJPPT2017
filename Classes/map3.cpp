#include"Map3.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool musicSet;
extern bool first;
extern bool soundSet;

Scene* Map3::createScene() {
	auto scene = Scene::create();
	auto layer = Map3::create();
	scene->addChild(layer);
	return scene;
}


bool Map3::init() {
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//创建外边款
	auto Map1Scene = Sprite::create("background/_battle.jpg");
	Map1Scene->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(Map1Scene, 0);

	//创建双人对战地图
	map3 = TMXTiledMap::create("map/map3/map3.tmx");
	map3->setAnchorPoint(Vec2(0.5, 0.5));
	map3->setPosition(Vec2(origin.x + visibleSize.width / 2 - 80, origin.y + visibleSize.height / 2));
	addChild(map3, 1);

	building = map3->getLayer("building");
	barrierLayer = map3->getLayer("barrier");

	//返回图片菜单
	auto returnItem = MenuItemImage::create(
		"button/backnormal.jpg",
		"button/backselect.jpg",
		CC_CALLBACK_1(Map3::menuReturnCallback, this));
	returnItem->setPosition(Vec2(origin.x + visibleSize.width - 85,
		origin.y + 25));
	auto menu = Menu::create(returnItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	//创建人物
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero/hero1Basic.plist");//缓存人物动画
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero/hero2Basic.plist");//缓存人物动画
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Bubble/bubble_all.plist");//缓存泡泡动画
	bubble = Bubble::create("bubble0.png");
	bubble->setAnchorPoint(Vec2(0.5f, 0.5f));
	bubble->setPosition(Vec2(origin.x, origin.y));
	this->addChild(bubble, 9);

	hero = Hero::create("hero1Down.png", 1);
	hero->setScene(building, barrierLayer, map3);
	bubble->setScene(building, barrierLayer, map3);
	hero->setPosition(Vec2(160, 50));
	this->addChild(hero, 10, HERO_1);

	hero2 = Hero::create("hero2Down.png", 2);
	hero2->setScene(building, barrierLayer, map3);
	hero2->setPosition(Vec2(480, 530));
	this->addChild(hero2, 10, HERO_2);

	bubble->player1 = hero;
	bubble->player2 = hero2;



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
		if (keyCode == EventKeyboard::KeyCode::KEY_ENTER && !hero->trapped) {
			bubble->placeBubble(hero->getCenterPosition(), hero);//完成放泡泡功能
		}
		if (hero->getAnimationPlaying() && !hero->trapped)//正在做的动画停止
		{
			hero->stopAllActions();
			hero->setAnimationPlaying(false);
		}

		if (checkArrow(keyCode) && !hero->trapped)
		{
			validPress1 = true;
			pressCnt1++;
			onPressCode1 = keyCode;
			keyCodeMap1[keyCode] = true;
		}
		if (hero2->trapped) return;
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
		if (checkArrow(keyCode) && !hero->trapped)
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
		if (hero2->trapped) return;
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
void Map3::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();
	if (musicSet) SimpleAudioEngine::getInstance()->playBackgroundMusic("music/gamestartmusic.mp3", true);
	first = true;
}

//清除场景
void Map3::cleanup() {
	Layer::cleanup();
	if (musicSet) {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic("music/vitory.mp3");
		SimpleAudioEngine::getInstance()->stopBackgroundMusic("music/gamestartmusic.mp3");
	}
}

//返回主界面
void Map3::menuReturnCallback(cocos2d::Ref* pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	Director::getInstance()->popScene();
}

void Map3::update(float dt)//每秒60次更新
{
	if (hero->die) {//?????
		hero2->win();
		hero->die = false;
		auto winHeroPic = Sprite::create("hero/hero2Win3.png");
		winHeroPic->setPosition(Vec2(250, 300));
		this->addChild(winHeroPic, 25);
		auto winPic = Sprite::create("hero/Win.png");
		winPic->setPosition(Vec2(400, 300));
		this->addChild(winPic, 20);
	}
	if (hero2->die) {
		hero->win();
		hero2->die = false;
		auto winHeroPic = Sprite::create("hero/hero1Win3.png");
		winHeroPic->setPosition(Vec2(250, 300));
		this->addChild(winHeroPic, 25);
		auto winPic = Sprite::create("hero/Win.png");
		winPic->setPosition(Vec2(400, 300));
		this->addChild(winPic, 20);
	}
	if (validPress1)
	{
		hero->moveHero(onPressCode1, bubble->bubblePos);
		hero->setAnimationPlaying(true);
	}

	if (validPress2)
	{
		hero2->moveHero(onPressCode2, bubble->bubblePos);
		hero2->setAnimationPlaying(true);
	}

}

void Map3::resetKeyCodeMap1()
{
	keyCodeMap1[EventKeyboard::KeyCode::KEY_LEFT_ARROW] = false;
	keyCodeMap1[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = false;
	keyCodeMap1[EventKeyboard::KeyCode::KEY_DOWN_ARROW] = false;
	keyCodeMap1[EventKeyboard::KeyCode::KEY_UP_ARROW] = false;

}

void Map3::resetKeyCodeMap2()
{
	keyCodeMap2[EventKeyboard::KeyCode::KEY_A] = false;
	keyCodeMap2[EventKeyboard::KeyCode::KEY_D] = false;
	keyCodeMap2[EventKeyboard::KeyCode::KEY_S] = false;
	keyCodeMap2[EventKeyboard::KeyCode::KEY_W] = false;

}

bool Map3::checkArrow(EventKeyboard::KeyCode keyCode)//检测P1按键
{
	return (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW
		|| keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW
		|| keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW
		|| keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW);
}

bool Map3::checkP2(EventKeyboard::KeyCode keyCode)//检测P2按键
{
	return (keyCode == EventKeyboard::KeyCode::KEY_A
		|| keyCode == EventKeyboard::KeyCode::KEY_D
		|| keyCode == EventKeyboard::KeyCode::KEY_W
		|| keyCode == EventKeyboard::KeyCode::KEY_S);
}

