#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "cocos2d.h"
#include"Hero.h"
#include"vector"
#include"queue"

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
	Hero* owner;
	int bubblePower;
	bool firstEnter;
	std::queue<float>xQueue;//记录每次放炮的像素位置
	std::queue<float>yQueue;

	std::vector<int>xVector;//记录所有尚未爆炸的泡泡的瓦片地图位置
	std::vector<int>yVector;//用来控制一个泡泡炸到另一个泡泡

public:
	void eraseFront();
	bool judgeBuilding(cocos2d::Vec2 pos);
	cocos2d::TMXLayer* building;
	cocos2d::TMXTiledMap* map;
	void setPointer(cocos2d::TMXLayer* buiding, cocos2d::TMXTiledMap* map);//标记他是在哪个场景
	static Bubble* create(const std::string &filename);
	void placeBubble(cocos2d::Vec2 pos,Hero* hero);
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 pos);
	cocos2d::Vec2 getPlacePosition(cocos2d::Vec2 pos);

};

#endif //__FIGHTER_SPRITE_H__
