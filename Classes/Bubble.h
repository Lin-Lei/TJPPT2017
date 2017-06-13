#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "cocos2d.h"
#include"Hero.h"
#include<list>
class bubbleInformation {
public:
	cocos2d::Sprite *bubble;
	Hero* hero;
	cocos2d::Vec2 position;//放置泡泡的像素坐标
	int power, tileX, tileY;//泡泡的威力和瓦片坐标
	bubbleInformation() {
		bubble = NULL;
		position = cocos2d::Vec2(0, 0);
		power = tileX = tileY = 0;
	}
	~bubbleInformation()=default;
	bubbleInformation& operator=(const bubbleInformation& a) {
		bubble = a.bubble;
		position = a.position;
		tileX = a.tileX;
		tileY = a.tileY;
		return *this;
	}
};

class Bubble : public cocos2d::Sprite
{
	void bubbleBoom(Hero * hero);
	void leftSpout(int power);
	void rightSpout(int power);
	void upSpout(int power);
	void downSpout(int power);
	void down(int power);
	void up(int power);
	void left(int power);
	void right(int power);
	std::list<bubbleInformation>bubbleInfo;
	void innitAnimation(cocos2d::Animation * a, int n, const char s[]);

	void resetPlaceBubbleNum(Hero* hero);
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 pos);
	cocos2d::Vec2 getPlacePosition(cocos2d::Vec2 pos,bubbleInformation *bInfo);
	void eraseFront();
	bool judgeBuilding(cocos2d::Vec2 pos);
	cocos2d::TMXLayer* building;
	cocos2d::TMXTiledMap* map;
	void boomInSameTime();

public:
	
	void setPointer(cocos2d::TMXLayer* buiding, cocos2d::TMXTiledMap* map);//标记他是在哪个场景
	static Bubble* create(const std::string &filename);
	void placeBubble(cocos2d::Vec2 pos,Hero* hero);
};

#endif //__FIGHTER_SPRITE_H__
