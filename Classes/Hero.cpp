#include "Hero.h"

USING_NS_CC;

Hero::Hero(int power, int speed, int number)
{
	setBubblePower(power);
	setMovingSpeed(speed);
	setBubbleNumber(number);
	setPlacedBubbleNum(0);

	animationPlaying = false;
}

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

void Hero::setPosition(const Vec2 &position)
{
	Size screenSize = Director::getInstance()->getVisibleSize();

	float Width = this->getContentSize().width ;
	float Height = this->getContentSize().height;
	float pos_x = position.x;
	float pos_y = position.y;

	if (pos_x < Width) {
		pos_x = Width;
	}
	else if (pos_x >(screenSize.width)) {
		pos_x = screenSize.width;
	}

	if (pos_y < Height) {
		pos_y = Height;
	}
	else if (pos_y >(screenSize.height)) {
		pos_y = screenSize.height;
	}

	Sprite::setPosition(Vec2(pos_x, pos_y));
	Sprite::setAnchorPoint(Vec2(0.5f, 0.5f));
}

void Hero::moveHero(const EventKeyboard::KeyCode keyCode)
{
	Vec2 position = this->getPosition();

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