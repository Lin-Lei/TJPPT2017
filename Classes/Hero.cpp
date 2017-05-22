#include "Hero.h"

USING_NS_CC;

extern TMXLayer* collidable;//检测碰撞
extern TMXTiledMap* oneTrainMap;//指向地图的

/*
人物锚点设置不当
由于锚点改变而引发的边界问题处理不当
getposition得到的是最外层边界
实际上我们只想要瓦片地图的边界
这里会出现负值导致触发断点
*/

Hero::Hero(int power, int speed, int number)
{
	bubblePower = power;
	setMovingSpeed(speed);
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

	float Width = this->getContentSize().width ;
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
	log("%f, %f", position.x, position.y);

	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		position.x -= movingSpeed;
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

	//从像素点坐标转化为瓦片坐标
	Vec2 tileCoord = this->tileCoordFromPosition(position);
	//获得瓦片的GID
	int tileGid = collidable->getTileGIDAt(tileCoord);

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

Vec2 Hero::tileCoordFromPosition(Vec2 position)//拿到的是人物在整个场景中的坐标
{
	int x = (position.x - 20) / oneTrainMap->getTileSize().width ;
	int y = ((oneTrainMap->getMapSize().height*oneTrainMap->getTileSize().height) - position.y + 40)
		/ oneTrainMap->getTileSize().height ;
	return Vec2(x, y);
}
