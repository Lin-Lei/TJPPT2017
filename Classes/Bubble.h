#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "cocos2d.h"
#include"Hero.h"

class Bubble : public cocos2d::Sprite
{
	void bubbleBoom(int power);
	void leftSpout(int power);
	void rightSpout(int power);
	void upSpout(int power);
	void downSpout(int power);
	void down(int power);
	void up(int power);
	void left(int power);
	void right(int power);
	void innitAnimation(cocos2d::Animation * a, int n, const char s[]);

public:
	static Bubble* create(const std::string &filename);
	void placeBubble(cocos2d::Vec2 pos,Hero* hero);
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 pos);
	cocos2d::Vec2 getPlacePosition(cocos2d::Vec2 pos);
};

#endif //__FIGHTER_SPRITE_H__
