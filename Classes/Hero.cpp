#include "Hero.h"

USING_NS_CC;

Hero::Hero(int power, int speed, int number)
{
	setBubblePower(power);
	setMovingSpeed(speed);
	setBubbleNumber(number);
}

Hero* Hero::create(const std::string &filename)
{
	Hero *hero = new Hero();

	if (hero && hero->initWithFile(filename)) {
		hero->autorelease();

		return hero;
	}

	CC_SAFE_DELETE(hero);

	return NULL;
}

void Hero::setPosition(const cocos2d::Vec2 &position)
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
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		position.x += movingSpeed;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		position.y -= movingSpeed;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		position.y += movingSpeed;
		break;

	default:
		break;
	}

	this->setPosition(position);
}