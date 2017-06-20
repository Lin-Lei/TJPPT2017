#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "cocos2d.h"
#include"Hero.h"
#include<list>
#include "SimpleAudioEngine.h"


class bubbleInformation {
public:
	cocos2d::Sprite *bubble;
	Hero* hero;
	cocos2d::Vec2 position;//�������ݵ���������
	int power, tileX, tileY;//���ݵ���������Ƭ����
	bool judge;
	bubbleInformation() {
		bubble = NULL;
		position = cocos2d::Vec2(0, 0);
		power = tileX = tileY = 0;
		judge = false;
	}
	~bubbleInformation() = default;
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
	void bubbleBoom(bubbleInformation  bubbleI);
	void down(bubbleInformation bubbleI);
	void up(bubbleInformation bubbleI);
	void left(bubbleInformation bubbleI);
	void right(bubbleInformation bubbleI);
	void innitAnimation(cocos2d::Animation * a, int n, const char s[]);

	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 pos);
	cocos2d::Vec2 getPlacePosition(cocos2d::Vec2 pos,bubbleInformation *bInfo,bubblePosition *bPos);
	void eraseLine(bubbleInformation bubbleI);
	bool judgeReBoom(cocos2d::Vec2 pos);
	bool judgeBuilding(cocos2d::Vec2 pos);
	cocos2d::TMXLayer* building;
	cocos2d::TMXTiledMap* map;
	cocos2d::TMXLayer* barrierLayer;
	void boomInSameTime(int x,int y);
	void judgeBoomHero(Hero* player,int x,int y);

public:
	std::list<bubbleInformation>bubbleInfo;
	std::list<bubblePosition>bubblePos;
	Hero* player1,* player2;
	void setScene(cocos2d::TMXLayer* buiding,cocos2d::TMXLayer* Barrier, cocos2d::TMXTiledMap* map);//����������ĸ�����
	static Bubble* create(const std::string &filename);
	void placeBubble(cocos2d::Vec2 pos,Hero* hero);
};

#endif //__FIGHTER_SPRITE_H__
