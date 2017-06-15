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
	cocos2d::TMXLayer* propLayer;
	int playerNo;
	
	void innitAnimation(cocos2d::Animation * a, int n, const char s[]);

public:
	bool trapped,die;
	void toDie();
	void toDie2();
	cocos2d::TMXLayer* building;
	cocos2d::TMXTiledMap* map;
	void win();
	void win2();
	void becomeDie();
	void becomeDie2();
	void setScene(cocos2d::TMXLayer* buiding, cocos2d::TMXTiledMap* map);//标记他是在哪个场景
	void judgeOnProps(const cocos2d::Vec2 pos);

	int bubblePower, bubbleNumber, placeBubbleNumber, movingSpeed;
	Hero(int playerNo = 1, int power = 1, int speed = 3, int number = 1);

	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 pos);
	static Hero* create(const std::string &filename, int playerNo = 1);
		
	void setPosition(const cocos2d::Vec2 &position);
	void moveHero(const cocos2d::EventKeyboard::KeyCode keyCode);//用于移动英雄
	void setFrame(const cocos2d::EventKeyboard::KeyCode keyCode);//英雄静止时重置图像

};

#endif //__FIGHTER_SPRITE_H__
