#include "Bubble.h"

USING_NS_CC;


Bubble* Bubble::create(const std::string& spriteFrameName)
{
	Bubble *bubble = new Bubble();
	bubble->player2 = NULL;
	if (bubble && bubble->initWithSpriteFrameName(spriteFrameName)) {
		bubble->autorelease();

		return bubble;
	}

	CC_SAFE_DELETE(bubble);

	return NULL;
}

void Bubble::setScene(TMXLayer* Building,TMXTiledMap* Map) {//����������ڳ���
	building = Building;
	map = Map;
}

Vec2 Bubble::tileCoordFromPosition(Vec2 position)//���������������������е�����,�õ�����ê�����ڵ���Ƭ����
{
	int x = (position.x - 20) / map->getTileSize().width;//��ȷ��
	int y = (map->getMapSize().height*map->getTileSize().height - position.y + 40)
		/ map->getTileSize().height;	//û�п���ê��
	return Vec2(x, y);
}

bool Bubble::judgeReBoom(Vec2 pos) {
	Vec2 tilePos = tileCoordFromPosition(pos);
	for (auto const &i : bubbleInfo) {
		//log("sx=%d sy=%d,x=%f y=%f", i.tileX, i.tileY, tilePos.x-1, tilePos.y);
		if (i.tileX == tilePos.x-1&&i.tileY == tilePos.y) return true;
	}
	return false;
}

void Bubble::resetPlaceBubbleNum(Hero *hero) {
	hero->placeBubbleNumber--;
}

Vec2 Bubble::getPlacePosition(Vec2 position,bubbleInformation *bInfo) {//�õ��������꣬�����������
	Vec2 post = tileCoordFromPosition(position);
	bInfo->tileX = post.x;
	bInfo->tileY = post.y;
	float x = post.x*map->getTileSize().width + map->getTileSize().width / 2 + 20 + this->getContentSize().width / 2;
	float y= (map->getMapSize().height - post.y -1)*map->getTileSize().height +//Ϊʲô��
		map->getTileSize().height/2 + 40 + this->getContentSize().height / 2-1;
	bInfo->position.x = x;
	bInfo->position.y = y;
	return Vec2(x, y);
}

void Bubble::innitAnimation(Animation * ani, int n, const char s[]) {//�ɹ��Ż���������,�����С��������
	for (int i = 1; i <= n; i++)
	{
		__String *FrameName = __String::createWithFormat("%s%d.png",s,i);
		SpriteFrame *bubbleFrame =
			SpriteFrameCache::getInstance()->getSpriteFrameByName(FrameName->getCString());
		ani->addSpriteFrame(bubbleFrame);
	}
}


//�жϽ����������������ͬʱ�ж��Ƿ������Ƭ��ͼ
bool Bubble::judgeBuilding(Vec2 pos) {
	Vec2 tileCoord = tileCoordFromPosition(pos);
	--tileCoord.x;//Ϊʲô��
	int x = tileCoord.x;
	int y = tileCoord.y;
	if (x<0||y<0||x>=15||y>=13) return true;//��������
	int tileGid = building->getTileGIDAt(tileCoord);
	if (tileGid) {//��������ɱ�ը�ٵĽ�����
		building->removeTileAt(tileCoord);
		return true;
	}
	return false;
}

void Bubble::judgeBoomHero(Hero *hero,int x,int y,int power) {
	if (hero->trapped) return;
	int heroX, heroY;
	Vec2 tilePos = tileCoordFromPosition(hero->getPosition());
	heroX = tilePos.x+0.3;//��ֹ�������
	heroY = tilePos.y+0.3;
	if (x == heroX) {
		if (abs(y - heroY) <= power) hero->becomeDie();
	}
	else if (y == heroY) {
		if (abs(x - heroX) <= power) hero->becomeDie();
	}
}
int c = 0;
void Bubble::boomInSameTime() {
	auto it = bubbleInfo.begin();
	auto first = bubbleInfo.begin();
	int x = bubbleInfo.begin()->tileX;
	int y = bubbleInfo.begin()->tileY;
	for (; it != bubbleInfo.end(); it++) {
		judgeBoomHero(player1, x, y,it->power);
		if(player2!=NULL) judgeBoomHero(player2, x, y, it->power);
		if (it->tileX == x&&it->tileY == y||it->judge) continue;//�����һ����ͷ���ͬһ��λ�õĵ�
		if (it->tileX == x) {
			if (abs(it->tileY - y) <= first->power) {
				auto temp = *first;
				*first = *it;
				it->tileX = temp.tileX;
				it->tileY = temp.tileY;
				it->judge = true;
				it->bubble->removeFromParent();
				bubbleBoom(it->hero);
			}
		}
		else if (it->tileY == y) {//else�������С���Ϊ���ݲ����ص�
			if (abs(it->tileX - x) <= first->power) {
				auto temp = *first;
				*first = *it;
				it->tileX = temp.tileX;
				it->tileY = temp.tileY;
				it->judge = true;
				it->bubble->removeFromParent();
				bubbleBoom(it->hero);
			}
		}
	}
}

void Bubble::eraseFront() {
	if(!bubbleInfo.empty())
		bubbleInfo.pop_front();
}

//��������
void Bubble::placeBubble(Vec2 p, Hero * hero) {//�õ�����������������
	if (hero->bubbleNumber == hero->placeBubbleNumber||hero->trapped) return;
	else {
		bubbleInformation bInfo;
		Vec2 position = getPlacePosition(p, &bInfo);
		for (const auto &i : bubbleInfo) {
			if (i.tileX == bInfo.tileX&&i.tileY == bInfo.tileY) return;
		}
		Sprite *bubble;
		bubble = Sprite::createWithSpriteFrameName("bubble1.png");
		bubble->setAnchorPoint(Vec2(0.5f, 0.5f));

		hero->placeBubbleNumber++;
		bInfo.power = hero->bubblePower;
		bInfo.hero = hero;
		bubble->setPosition(position);//���λ�úͳ������ݵ�λ���й�
		this->addChild(bubble);
		bInfo.bubble = bubble;
		bubbleInfo.push_back(bInfo);
		
		Animation *bubbleAnimation = Animation::create();//���ó���������������
		innitAnimation(bubbleAnimation, 3, "bubble");
		bubbleAnimation->setDelayPerUnit(0.15f);//��֮֡���ӳ�
		Animate *bubbleAnimate = Animate::create(bubbleAnimation);
		
		auto bubbleBoomFunc = CallFunc::create(CC_CALLBACK_0(Bubble::bubbleBoom, this, hero));//�������ݱ�ը�ص�����
		auto delayTime = DelayTime::create(0.001f);
		auto bubbleAction = Sequence::create(Repeat::create(bubbleAnimate, 5),bubbleBoomFunc, 
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)),NULL);
		bubble->runAction(bubbleAction);//ͨ��removeFromParent�����˲���ͼƬ
		}
}

void Bubble::bubbleBoom(Hero* hero) {
	c++;
	hero->placeBubbleNumber--;
	Sprite * bubble;
	bubble = Sprite::createWithSpriteFrameName("bubbleCenter1.png");
	//log("size=%d", bubbleInfo.size());
	bubble->setPosition(bubbleInfo.front().position);//����������ը+��ը��������bug
	this->addChild(bubble);
	Animation *bubbleCenterAnimation = Animation::create();
	innitAnimation(bubbleCenterAnimation, 4, "bubbleCenter");
	bubbleCenterAnimation->setDelayPerUnit(0.04f);//��֮֡���ӳ�
	Animate *bubbleCenterAnimate = Animate::create(bubbleCenterAnimation);//���������м䱬ը����

	auto upAction = CallFunc::create(CC_CALLBACK_0(Bubble::up, this, bubbleInfo.front().position));
	auto rightAction = CallFunc::create(CC_CALLBACK_0(Bubble::right, this, bubbleInfo.front().position));
	auto leftAction = CallFunc::create(CC_CALLBACK_0(Bubble::left, this, bubbleInfo.front().position));
	auto downAction = CallFunc::create(CC_CALLBACK_0(Bubble::down, this, bubbleInfo.front().position));
	auto boomAction = CallFunc::create(CC_CALLBACK_0(Bubble::boomInSameTime, this));
	auto spawnAction = Spawn::create(bubbleCenterAnimate,downAction,upAction,
		leftAction,rightAction,boomAction,NULL);
	auto delayTime = DelayTime::create(0.01f);
	auto action = Sequence::create(spawnAction,delayTime, CallFunc::create(CC_CALLBACK_0(Bubble::eraseFront,this)),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent,bubble)),NULL);
	bubble->runAction(action);
}


void Bubble::down(Vec2 pos) {
	if (bubbleInfo.front().power > 1) {
		for (int p = 1; p < bubbleInfo.front().power; p++) {
			if (judgeReBoom(Vec2(pos.x, pos.y - 40 * p))||judgeBuilding(Vec2(pos.x, pos.y - 40 * p))) return;
			Sprite * temp;
			temp = Sprite::createWithSpriteFrameName("down1.png");
			temp->setPosition(Vec2(pos.x, pos.y - 40 * p));
			this->addChild(temp);
			Animation *tempAnimation = Animation::create();
			innitAnimation(tempAnimation, 2, "down");
			tempAnimation->setDelayPerUnit(0.02f);
			Animate *tempAnimate = Animate::create(tempAnimation);
			auto tempAction =Repeat::create(tempAnimate, 4);
			auto delayTime = DelayTime::create(0.001f);
			auto bubbleAction = Sequence::create(tempAction, delayTime,
				CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)), NULL);
			temp->runAction(bubbleAction);
		}
	}
	if (judgeBuilding(Vec2(pos.x, pos.y - 40 * bubbleInfo.front().power))) return;
	Sprite * bubble;
	bubble = Sprite::createWithSpriteFrameName("downSpout1.png");
	bubble->setPosition(Vec2(pos.x, pos.y - 40 * bubbleInfo.front().power));
	this->addChild(bubble);
	Animation *bubbleAnimation = Animation::create();
	innitAnimation(bubbleAnimation, 12, "downSpout");
	bubbleAnimation->setDelayPerUnit(0.015f);//��֮֡���ӳ�
	Animate *bubbleAnimate = Animate::create(bubbleAnimation);
	auto delayTime = DelayTime::create(0.001f);
	auto action = Sequence::create(bubbleAnimate, delayTime,
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)), NULL);
	bubble->runAction(action);
}

void Bubble::up(Vec2 pos) {
	if (bubbleInfo.front().power > 1) {
		for (int p = 1; p < bubbleInfo.front().power; p++) {
			if (judgeReBoom(Vec2(pos.x, pos.y + 40 * p))||judgeBuilding(Vec2(pos.x, pos.y + 40 * p))) return;
			Sprite * temp;
			temp = Sprite::createWithSpriteFrameName("up1.png");
			temp->setPosition(Vec2(pos.x, pos.y + 40 * p));
			this->addChild(temp);
			Animation *tempAnimation = Animation::create();
			innitAnimation(tempAnimation, 2, "up");
			tempAnimation->setDelayPerUnit(0.02f);
			Animate *tempAnimate = Animate::create(tempAnimation);
			auto tempAction = Repeat::create(tempAnimate, 4);
			auto delayTime = DelayTime::create(0.001f);
			auto bubbleAction = Sequence::create(tempAction, delayTime,
				CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)), NULL);
			temp->runAction(bubbleAction);
		}
	}
	if (judgeBuilding(Vec2(pos.x,pos.y + 40 * bubbleInfo.front().power))) return;
	Sprite * bubble;	
	bubble = Sprite::createWithSpriteFrameName("upSpout1.png");
	bubble->setPosition(Vec2(pos.x,pos.y + 40 * bubbleInfo.front().power));
	this->addChild(bubble);
	Animation *bubbleAnimation = Animation::create();
	innitAnimation(bubbleAnimation, 12, "upSpout");
	bubbleAnimation->setDelayPerUnit(0.015f);//��֮֡���ӳ�
	Animate *bubbleAnimate = Animate::create(bubbleAnimation);
	auto delayTime = DelayTime::create(0.001f);
	auto action = Sequence::create(bubbleAnimate, delayTime,
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)), NULL);
	bubble->runAction(action);
}

void Bubble::left(Vec2 pos) {
	if (bubbleInfo.front().power > 1) {
		for (int p = 1; p < bubbleInfo.front().power; p++) {
			if (judgeReBoom(Vec2(pos.x - 40 * p, pos.y))||judgeBuilding(Vec2(pos.x - 40 * p, pos.y))) return;
			Sprite * temp;
			temp = Sprite::createWithSpriteFrameName("left1.png");
			temp->setPosition(Vec2(pos.x - 40 * p, pos.y));
			this->addChild(temp);
			Animation *tempAnimation = Animation::create();
			innitAnimation(tempAnimation, 2, "left");
			tempAnimation->setDelayPerUnit(0.02f);
			Animate *tempAnimate = Animate::create(tempAnimation);
			auto tempAction = Repeat::create(tempAnimate, 4);
			auto delayTime = DelayTime::create(0.001f);
			auto bubbleAction = Sequence::create(tempAction, delayTime,
				CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)), NULL);
			temp->runAction(bubbleAction);
		}
	}
	if (judgeBuilding(Vec2(pos.x - 40 * bubbleInfo.front().power,pos.y))) return;
	Sprite *bubble;
	bubble = Sprite::createWithSpriteFrameName("leftSpout1.png");
	bubble->setPosition(Vec2(pos.x - 40 * bubbleInfo.front().power,pos.y));
	this->addChild(bubble);
	Animation *bubbleLeftAnimation = Animation::create();
	innitAnimation(bubbleLeftAnimation, 12, "leftSpout");
	bubbleLeftAnimation->setDelayPerUnit(0.015f);//��֮֡���ӳ�
	Animate *bubbleAnimate = Animate::create(bubbleLeftAnimation);
	auto delayTime = DelayTime::create(0.001f);
	auto action = Sequence::create(bubbleAnimate, delayTime,
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)), NULL);
	bubble->runAction(action);
}

void Bubble::right(Vec2 pos) {
	if (bubbleInfo.front().power > 1) {
		for (int p = 1; p < bubbleInfo.front().power; p++) {
			if (judgeReBoom(Vec2(pos.x + 40 * p, pos.y))||judgeBuilding(Vec2(pos.x + 40 * p, pos.y))) return;
			Sprite * temp;
			temp = Sprite::createWithSpriteFrameName("right1.png");
			temp->setPosition(Vec2(pos.x + 40 * p, pos.y));
			this->addChild(temp);
			Animation *tempAnimation = Animation::create();
			innitAnimation(tempAnimation, 2, "right");
			tempAnimation->setDelayPerUnit(0.02f);
			Animate *tempAnimate = Animate::create(tempAnimation);
			auto tempAction = Repeat::create(tempAnimate, 4);
			auto delayTime = DelayTime::create(0.001f);
			auto bubbleAction = Sequence::create(tempAction, delayTime,
				CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)), NULL);
			temp->runAction(bubbleAction);
		}
	}
	if (judgeBuilding(Vec2(pos.x + 40 * bubbleInfo.front().power,pos.y))) return;
	Sprite * bubble;
	bubble = Sprite::createWithSpriteFrameName("rightSpout1.png");
	bubble->setPosition(Vec2(pos.x + 40 * bubbleInfo.front().power,pos.y));
	this->addChild(bubble);
	Animation *bubbleAnimation = Animation::create();
	innitAnimation(bubbleAnimation, 12, "rightSpout");
	bubbleAnimation->setDelayPerUnit(0.015f);//��֮֡���ӳ�
	Animate *bubbleAnimate = Animate::create(bubbleAnimation);
	auto delayTime = DelayTime::create(0.001f);
	auto action = Sequence::create(bubbleAnimate, delayTime,
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)),NULL);
	bubble->runAction(action);
}

