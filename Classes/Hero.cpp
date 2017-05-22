#include "Hero.h"

USING_NS_CC;

extern TMXLayer* building;
extern TMXTiledMap* oneTrainMap;//ָ���ͼ��

Vec2 Hero::tileCoordFromPosition(Vec2 position)//�õ��������������������е�����,�����Ƭ����
{
	int x = (position.x + 10) / oneTrainMap->getTileSize().width;
	int y = ((oneTrainMap->getMapSize().height*oneTrainMap->getTileSize().height) - position.y + 40)
		/ oneTrainMap->getTileSize().height + 1;
	return Vec2(x, y);
}


Hero::Hero(int power, int speed, int number)
{
	bubblePower = power;
	movingSpeed=speed;
	bubbleNumber = number;
	placeBubbleNumber = 0;

	animationPlaying = false;

	shoseLayer = oneTrainMap->getLayer("speed");
	powerLayer = oneTrainMap->getLayer("bubblePower");
	numLayer = oneTrainMap->getLayer("bubbleNum");
}

//��������
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

//����λ��
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
	Sprite::setAnchorPoint(Vec2(0.5f, 0.1f));//����ê����Ҫ�Ľ����߽�����
}

//�����ƶ�
void Hero::moveHero(const EventKeyboard::KeyCode keyCode)
{
	Vec2 position = this->getPosition();
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
		break;

	default:
		break;
	}


	//�����ص�����ת��Ϊ��Ƭ����
	Vec2 tileCoord = tileCoordFromPosition(position);
	//�����Ƭ��GID
	int tileGid = building->getTileGIDAt(Vec2(tileCoord.x-1,tileCoord.y-1));
	if (tileGid > 0) {
		position = this->getPosition();
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
	int tileGid= powerLayer->getTileGIDAt(Vec2(tileCoord.x - 1, tileCoord.y - 1));
	if (tileGid) {
		bubblePower++;
		powerLayer->removeTileAt(Vec2(tileCoord.x-1,tileCoord.y-1));
	}

	tileGid = numLayer->getTileGIDAt(Vec2(tileCoord.x - 1, tileCoord.y - 1));

	if (tileGid) {
		bubbleNumber++;
		numLayer->removeTileAt(Vec2(tileCoord.x - 1, tileCoord.y - 1));
	}

	tileGid = shoseLayer->getTileGIDAt(Vec2(tileCoord.x - 1, tileCoord.y - 1));

	if (tileGid) {
		movingSpeed++;
		shoseLayer->removeTileAt(Vec2(tileCoord.x - 1, tileCoord.y - 1));
	}
}
