#include"OneTrain.h"
#include"Vector"

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

	//������߿�
	auto oneTrainScene = Sprite::create("background/_battle.jpg");
	oneTrainScene->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(oneTrainScene,0);

	//��������ѵ����ͼ
	oneTrainMap = TMXTiledMap::create("map/map1/map1.tmx");
	oneTrainMap->setAnchorPoint(Vec2(0.5, 0.5));
	oneTrainMap->setPosition(Vec2(origin.x + visibleSize.width / 2-80, origin.y + visibleSize.height / 2));
	addChild(oneTrainMap, 1);
	
	building = oneTrainMap->getLayer("building");
	barrierLayer = oneTrainMap->getLayer("barrier");
	
	//����ͼƬ�˵�
	auto returnItem = MenuItemImage::create(
		"button/backnormal.jpg",
		"button/backselect.jpg",
		CC_CALLBACK_1(OneTrain::menuReturnCallback, this));
	returnItem->setPosition(Vec2(origin.x + visibleSize.width -85,
		origin.y + 25));
	auto menu = Menu::create (returnItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	//��������
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero/hero1Basic.plist");//�������ﶯ��
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Bubble/bubble_all.plist");//�������ݶ���
	bubble = Bubble::create("bubble0.png");
	bubble->setAnchorPoint(Vec2(0.5f,0.5f));
	bubble->setPosition(Vec2(origin.x , origin.y ));
	this->addChild(bubble, 9);
	hero = Hero::create("hero1Down.png");
	hero->setScene(building, barrierLayer, oneTrainMap);
	bubble->setScene(building, barrierLayer, oneTrainMap);
	hero->setPosition(Vec2(160,50));
	this->addChild(hero, 10, HERO_1);

	bubble->player1 = hero;

	//��ʼ��Map����
	keyCodeMap = std::map<cocos2d::EventKeyboard::KeyCode, bool>();
	resetKeyCodeMap();
	
	pressCnt = 0; //�Ż��ఴ��ʱ���ж�
	validPress = false;

	//ע�� �����¼�������
	heroKeyboardListener = EventListenerKeyboard::create();
	
	//����ʱ����
	heroKeyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE && !hero->trapped) {
			bubble->placeBubble(hero->getCenterPosition(),hero);//��ɷ����ݹ���
		}
		if (hero->getAnimationPlaying() && !hero->trapped)//�������Ķ���ֹͣ
		{
			hero->stopAllActions();
			hero->setAnimationPlaying(false);
		}
		
		if (checkArrow(keyCode) && !hero->trapped)
		{
			validPress = true;
			pressCnt++;
			onPressCode = keyCode;
			keyCodeMap[keyCode] = true;
		}
	};

	//�ɿ�ʱ����
	heroKeyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (checkArrow(keyCode) && !hero->trapped)
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

//������Ϸ����
void OneTrain::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();
	if(musicSet) SimpleAudioEngine::getInstance()->playBackgroundMusic("music/gamestartmusic.mp3",true);
	first = true;
}

//�������
void OneTrain::cleanup() {
	Layer::cleanup();
	if(musicSet) SimpleAudioEngine::getInstance()->stopBackgroundMusic("music/gamestartmusic.mp3");
}

//����������
void OneTrain::menuReturnCallback(cocos2d::Ref* pSender) {
	if (soundSet) SimpleAudioEngine::getInstance()->playEffect("music/clip.mp3");
	Director::getInstance()->popScene();
}

void OneTrain::update(float dt)//ÿ��60�θ���
{

	if (validPress)
	{
		hero->moveHero(onPressCode, bubble->bubblePos);
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

