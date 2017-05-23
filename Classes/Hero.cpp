#include "Hero.h"

USING_NS_CC;


Vec2 Hero::tileCoordFromPosition(Vec2 position)//拿到的是人物在整个场景中的坐标,输出瓦片坐标
{
	int x = (position.x - 20) / map->getTileSize().width;
	int y = ((map->getMapSize().height*map->getTileSize().height) - position.y + 40)
		/ map->getTileSize().height;
	return Vec2(x, y);
}

void Hero::setPointer(TMXLayer* Building,TMXTiledMap* Map) {
	building = Building;
	map = Map;
	shoseLayer = map->getLayer("speed");
	powerLayer = map->getLayer("bubblePower");
	numLayer = map->getLayer("bubbleNum");
}


Hero::Hero(int power, int speed, int number)
{
	bubblePower = power;
	movingSpeed=speed;
	bubbleNumber = number;
	placeBubbleNumber = 0;

	animationPlaying = false;
}

//创建人物
Hero* Hero::create(const std::string& spriteFrameName)
{
	Hero *hero = new Hero();

	if (hero && hero->initWithSpriteFrameName(spriteFrameName)) {
		hero->autorelease();

		return hero;
	}

	CC_SAFE_DELETE(hero);

	return NULL;
}

//设置位置
void Hero::setPosition(const Vec2 &position)
{
	Size screenSize = Director::getInstance()->getVisibleSize();

	float Width = this->getContentSize().width;
	float Height = this->getContentSize().height;
	float pos_x = position.x;
	float pos_y = position.y;

	if (pos_x < 20 + Width / 2) {
		pos_x = 20 + Width / 2;
	}
	else if (pos_x >620 - Width / 2) {
		pos_x = 620 - Width / 2;
	}

	if (pos_y < 40 + Height / 10) {
		pos_y = 40 + Height / 10;
	}
	else if (pos_y >560 - Height * 0.5) {
		pos_y = 560 - Height * 0.5;
	}



	Sprite::setPosition(Vec2(pos_x, pos_y));
	Sprite::setAnchorPoint(Vec2(0.5f, 0.1f));//人物锚点需要改进，边界问题
}

//人物移动
void Hero::moveHero(const EventKeyboard::KeyCode keyCode)
{
	Vec2 position = this->getPosition();

	Vec2 collisionPos1 = position; //允许1像素的优化
	Vec2 collisionPos2 = position;
	Vec2 tileCoord1;
	Vec2 tileCoord2;
	int tileGid1;
	int tileGid2;



	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		position.x -= movingSpeed;
		judgeOnProps(position);
		if (!animationPlaying)
		{
			Animation *moveLeftAnimation = Animation::create();
			for (int i = 1; i <= 4; i++)
			{
				__String *frameName = __String::createWithFormat("hero1Left%d.png", i);
				SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
				moveLeftAnimation->addSpriteFrame(spriteFrame);
			}
			moveLeftAnimation->setDelayPerUnit(0.1f);
			Animate *moveLeftAnimate = Animate::create(moveLeftAnimation);
			runAction(RepeatForever::create(moveLeftAnimate));
		}

		collisionPos1.x = position.x - map->getTileSize().width / 2;
		collisionPos1.y = position.y - this->getContentSize().height*0.1 + map->getTileSize().height-2;
		collisionPos2.x = position.x - map->getTileSize().width / 2;
		collisionPos2.y = position.y - this->getContentSize().height*0.1+2;
		tileCoord1 = tileCoordFromPosition(collisionPos1);
		tileCoord2 = tileCoordFromPosition(collisionPos2);
		tileGid1 = building->getTileGIDAt(tileCoord1);
		tileGid2 = building->getTileGIDAt(tileCoord2);

		if (tileGid1 || tileGid2) 
		{
			position = this->getPosition();
		}
		break;
		
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		
		position.x += movingSpeed;
		judgeOnProps(position);
		if (!animationPlaying)
		{
			Animation *moveRightAnimation = Animation::create();
			for (int i = 1; i <= 4; i++)
			{
				__String *frameName = __String::createWithFormat("hero1Right%d.png", i);
				SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
				moveRightAnimation->addSpriteFrame(spriteFrame);
			}
			moveRightAnimation->setDelayPerUnit(0.1f);
			Animate *moveRightAnimate = Animate::create(moveRightAnimation);
			runAction(RepeatForever::create(moveRightAnimate));
		}

		collisionPos1.x = position.x + map->getTileSize().width / 2;
		collisionPos1.y = position.y - this->getContentSize().height*0.1 + map->getTileSize().height-2;
		collisionPos2.x = position.x + map->getTileSize().width / 2;
		collisionPos2.y = position.y - this->getContentSize().height*0.1+2;
		tileCoord1 = tileCoordFromPosition(collisionPos1);
		tileCoord2 = tileCoordFromPosition(collisionPos2);
		tileGid1 = building->getTileGIDAt(tileCoord1);
		tileGid2 = building->getTileGIDAt(tileCoord2);

		if (tileGid1 || tileGid2)
		{
			position = this->getPosition();
		}
		break;
		
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		position.y -= movingSpeed;
		judgeOnProps(position);
		if (!animationPlaying)
		{
			Animation *moveDownAnimation = Animation::create();
			for (int i = 1; i <= 4; i++)
			{
				__String *frameName = __String::createWithFormat("hero1Down%d.png", i);
				SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
				moveDownAnimation->addSpriteFrame(spriteFrame);
			}
			moveDownAnimation->setDelayPerUnit(0.1f);
			Animate *moveDownAnimate = Animate::create(moveDownAnimation);
			runAction(RepeatForever::create(moveDownAnimate));
		}

		collisionPos1.x = position.x + map->getTileSize().width / 2-2;
		collisionPos1.y = position.y - this->getContentSize().height*0.1;
		collisionPos2.x = position.x - map->getTileSize().width / 2+2;
		collisionPos2.y = position.y - this->getContentSize().height*0.1;
		tileCoord1 = tileCoordFromPosition(collisionPos1);
		tileCoord2 = tileCoordFromPosition(collisionPos2);
		tileGid1 = building->getTileGIDAt(tileCoord1);
		tileGid2 = building->getTileGIDAt(tileCoord2);

		if (tileGid1 || tileGid2)
		{
			position = this->getPosition();
		}
		break;
		
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		position.y += movingSpeed;
		judgeOnProps(position);
		if (!animationPlaying)
		{
			Animation *moveUpAnimation = Animation::create();
			for (int i = 1; i <= 4; i++)
			{
				__String *frameName = __String::createWithFormat("hero1Up%d.png", i);
				SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
				moveUpAnimation->addSpriteFrame(spriteFrame);
			}
			moveUpAnimation->setDelayPerUnit(0.1f);
			moveUpAnimation->setRestoreOriginalFrame(true);
			Animate *moveUpAnimate = Animate::create(moveUpAnimation);
			runAction(RepeatForever::create(moveUpAnimate));
		}

		collisionPos1.x = position.x + map->getTileSize().width / 2-2;
		collisionPos1.y = position.y - this->getContentSize().height*0.1 + map->getTileSize().height;
		collisionPos2.x = position.x - map->getTileSize().width / 2+2;
		collisionPos2.y = position.y - this->getContentSize().height*0.1 + map->getTileSize().height;
		tileCoord1 = tileCoordFromPosition(collisionPos1);
		tileCoord2 = tileCoordFromPosition(collisionPos2);
		tileGid1 = building->getTileGIDAt(tileCoord1);
		tileGid2 = building->getTileGIDAt(tileCoord2);

		if (tileGid1 || tileGid2)
		{
			position = this->getPosition();
		}
		break;

	default:
		break;
	}


	this->setPosition(position);
}

void Hero::setFrame(const cocos2d::EventKeyboard::KeyCode keyCode)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	{
		SpriteFrame *hero1Left = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1Left.png");
		setSpriteFrame(hero1Left);
	}
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	{
		SpriteFrame *hero1Right = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1Right.png");
		setSpriteFrame(hero1Right);
	}
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	{
		SpriteFrame *hero1Down = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1Down.png");
		setSpriteFrame(hero1Down);
	}
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	{
		SpriteFrame *hero1Up = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1Up.png");
		setSpriteFrame(hero1Up);
	}
		break;

	default:
		break;
	}
}

void Hero::judgeOnProps(const Vec2 pos) {
	Vec2 tileCoord = tileCoordFromPosition(pos);
	int tileGid= powerLayer->getTileGIDAt(tileCoord);
	if (tileGid) {
		bubblePower++;
		powerLayer->removeTileAt(tileCoord);
	}

	tileGid = numLayer->getTileGIDAt(tileCoord);

	if (tileGid) {
		bubbleNumber++;
		numLayer->removeTileAt(tileCoord);
	}

	tileGid = shoseLayer->getTileGIDAt(tileCoord);

	if (tileGid) {
		movingSpeed++;
		shoseLayer->removeTileAt(tileCoord);
	}
}
