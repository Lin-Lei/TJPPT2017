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


class bubblePosition{
public:
	int tileX, tileY;
	cocos2d::Vec2 position;
};


class Hero : public cocos2d::Sprite
{
	CC_SYNTHESIZE(bool, animationPlaying, AnimationPlaying);
	cocos2d::TMXLayer* propLayer;
	int playerNo;
	cocos2d::Vec2 heroPosition;//用于人物绘制
	cocos2d::Vec2 centerPosition;//用于逻辑判断
	void innitAnimation(cocos2d::Animation * a, int n, const char s[]);

public:
	bool trapped,die;
	void toDie();
	cocos2d::TMXLayer* barrierLayer;
	cocos2d::TMXLayer* building;
	cocos2d::TMXTiledMap* map;
	void win();
	void becomeDie();
	void setScene(cocos2d::TMXLayer* buiding ,cocos2d::TMXLayer* barrier,cocos2d::TMXTiledMap* map);//标记他是在哪个场景
	void judgeOnProps(const cocos2d::Vec2 pos);

	int bubblePower, bubbleNumber, placeBubbleNumber;
	float movingSpeed;
	Hero(int playerNo = 1, int power = 1, float speed = 2, int number = 1);

	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 pos);
	static Hero* create(const std::string &filename, int playerNo = 1);
		
	void setPosition(const cocos2d::Vec2 &position);
	cocos2d::Vec2 getPosition();//用于人物绘制
	cocos2d::Vec2 getCenterPosition();//用于逻辑判断
	void moveHero(const cocos2d::EventKeyboard::KeyCode keyCode, std::list<bubblePosition>bubblePos);//用于移动英雄
	void setFrame(const cocos2d::EventKeyboard::KeyCode keyCode);//英雄静止时重置图像

	bool checkBubble(cocos2d::Vec2 position, std::list<bubblePosition>bubblePos);

	int countCol(int tileGid1, int tileGid2, bool bubble1, bool bubble2, int tileGid3, int tileGid4);

};

#endif //__FIGHTER_SPRITE_H__
