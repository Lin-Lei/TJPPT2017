#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"

typedef enum
{
	HERO_1 = 1,
	HERO_2,
	HERO_3,
	HERO_4,
	HERO_5,
	HERO_6,
	HERO_7,
	HERO_8
} HeroTag;

class Hero : public cocos2d::Sprite
{
	CC_SYNTHESIZE(int, bubblePower, BubblePower);
	CC_SYNTHESIZE(int, movingSpeed, MovingSpeed);
	CC_SYNTHESIZE(int, bubbleNumber, BubbleNumber);
	CC_SYNTHESIZE(int, placedBubbleNum, PlacedBubbleNum);
	CC_SYNTHESIZE(bool, animationPlaying, AnimationPlaying);

	

public:
	
	Hero(int power = 1, int speed = 1, int number = 1);

	static Hero* create(const std::string &filename);

	void setPosition(const cocos2d::Vec2 &position);
	void moveHero(const cocos2d::EventKeyboard::KeyCode keyCode);
	void setFrame(const cocos2d::EventKeyboard::KeyCode keyCode);
	void Hero::placeBubble(const cocos2d::Vec2 &position);

};

#endif //__FIGHTER_SPRITE_H__
