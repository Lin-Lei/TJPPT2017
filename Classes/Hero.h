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
	CC_SYNTHESIZE(bool, animationPlaying, AnimationPlaying);
	cocos2d::TMXLayer* shoseLayer;
	cocos2d::TMXLayer* powerLayer;
	cocos2d::TMXLayer* numLayer;
public:
	
	void judgeOnProps(const cocos2d::Vec2 pos);
	Hero(int power = 1, int speed = 3, int number = 1);
	int bubblePower, bubbleNumber, placeBubbleNumber, movingSpeed;
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 pos);
	static Hero* create(const std::string &filename);
		
	void setPosition(const cocos2d::Vec2 &position);
	void moveHero(const cocos2d::EventKeyboard::KeyCode keyCode);
	void setFrame(const cocos2d::EventKeyboard::KeyCode keyCode);
};

#endif //__FIGHTER_SPRITE_H__
