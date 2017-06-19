#include "Bubble.h"

USING_NS_CC;
using namespace CocosDenshion;

extern bool musicSet;
extern bool soundSet;

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

void Bubble::setScene(TMXLayer* Building,TMXLayer* Barrier,TMXTiledMap* Map) {//����������ڳ���
	building = Building;
	map = Map;
	barrierLayer = Barrier;
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
		if (i.tileX == tilePos.x-1&&i.tileY == tilePos.y) return true;
	}
	return false;
}


Vec2 Bubble::getPlacePosition(Vec2 position,bubbleInformation *bInfo,bubblePosition *bPos) {//�õ��������꣬�����������
	Vec2 post = tileCoordFromPosition(position);
	bPos->tileX = bInfo->tileX = post.x;
	bPos->tileY = bInfo->tileY = post.y;
	
	float x = post.x*map->getTileSize().width + map->getTileSize().width / 2 + 20 + this->getContentSize().width / 2;
	float y= (map->getMapSize().height - post.y -1)*map->getTileSize().height +//Ϊʲô��
		map->getTileSize().height/2 + 40 + this->getContentSize().height / 2-1;
	bPos->position.x=bInfo->position.x = x;
	bPos->position.y=bInfo->position.y = y;
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
	tileGid = barrierLayer->getTileGIDAt(tileCoord);
	if (tileGid) return true;
	return false;
}

void Bubble::judgeBoomHero(Hero *hero,int x,int y) {
	if (hero->trapped) return;
	log("boomherox=%d y=%d", x, y);
	int heroX, heroY;
	Vec2 tilePos = tileCoordFromPosition(hero->centerPosition);
	heroX = tilePos.x+0.3;//��ֹ�������
	heroY = tilePos.y+0.3;
	if (x == heroX&&y==heroY) {
		hero->becomeDie();
	}
}

void Bubble::boomInSameTime(int x,int y) {//��bug
	auto it = bubbleInfo.begin();
	for (; it != bubbleInfo.end(); it++) { 
		if (it->tileX == x&&it->tileY==y&&!it->judge) {
			//log("same");
			it->bubble->removeFromParent();//��placebubble�����remove�����ظ���
			bubbleBoom(*it);
		}
	}
}

void Bubble::eraseLine(bubbleInformation bInfo) {
	auto it = bubbleInfo.begin();
	auto itP = bubblePos.begin();
	for (; it != bubbleInfo.end(); it++,itP++) {
		if (it->tileX==bInfo.tileX&&it->tileY==bInfo.tileY) {
			bubbleInfo.erase(it);
			bubblePos.erase(itP);
			return;
		}
	}
}

//��������
void Bubble::placeBubble(Vec2 p, Hero * hero) {//�õ�����������������
	if (hero->bubbleNumber == hero->placeBubbleNumber||hero->trapped) return;
	else {
		bubbleInformation bInfo;
		bubblePosition bPos;
		Vec2 position = getPlacePosition(p, &bInfo,&bPos);
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
		bubblePos.push_back(bPos);
		
		Animation *bubbleAnimation = Animation::create();//���ó���������������
		innitAnimation(bubbleAnimation, 3, "bubble");
		bubbleAnimation->setDelayPerUnit(0.15f);//��֮֡���ӳ�
		Animate *bubbleAnimate = Animate::create(bubbleAnimation);
		
		auto bubbleBoomFunc = CallFunc::create(CC_CALLBACK_0(Bubble::bubbleBoom, this, bInfo));//�������ݱ�ը�ص�����
		auto delayTime = DelayTime::create(0.001f);
		auto bubbleAction = Sequence::create(Repeat::create(bubbleAnimate, 5),bubbleBoomFunc, 
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)),NULL);
		bubble->runAction(bubbleAction);//ͨ��removeFromParent�����˲���ͼƬ
		}
}

void Bubble::bubbleBoom(bubbleInformation bInfo) {
	bInfo.hero->placeBubbleNumber--;
	
	Sprite * bubble;
	bubble = Sprite::createWithSpriteFrameName("bubbleCenter1.png");
	bubble->setPosition(bInfo.position);
	this->addChild(bubble);
	Animation *bubbleCenterAnimation = Animation::create();
	innitAnimation(bubbleCenterAnimation, 4, "bubbleCenter");
	bubbleCenterAnimation->setDelayPerUnit(0.04f);//��֮֡���ӳ�
	Animate *bubbleCenterAnimate = Animate::create(bubbleCenterAnimation);//���������м䱬ը����

	auto upAction = CallFunc::create(CC_CALLBACK_0(Bubble::up, this, bInfo));
	auto rightAction = CallFunc::create(CC_CALLBACK_0(Bubble::right, this, bInfo));
	auto leftAction = CallFunc::create(CC_CALLBACK_0(Bubble::left, this, bInfo));
	auto downAction = CallFunc::create(CC_CALLBACK_0(Bubble::down, this, bInfo));
	auto spawnAction = Spawn::create(bubbleCenterAnimate,downAction,upAction,
		leftAction,rightAction,NULL);
	auto delayTime = DelayTime::create(0.01f);
	auto action = Sequence::create(spawnAction,delayTime,CallFunc::create(CC_CALLBACK_0(Bubble::eraseLine,this, bInfo)),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent,bubble)),NULL);
	judgeBoomHero(player1, bInfo.tileX, bInfo.tileY);
	if (player2 != NULL)  judgeBoomHero(player2, bInfo.tileX, bInfo.tileY);
	for (auto &i : bubbleInfo) {
		if (i.tileX == bInfo.tileX&&i.tileY == bInfo.tileY) i.judge = true;
	}
	bubble->runAction(action);
	if(soundSet) SimpleAudioEngine::getInstance()->playEffect("music/boom.mp3");
}


void Bubble::down(bubbleInformation bInfo) {//�����Ƿ���
	if (bInfo.power > 1) {
		for (int p = 1; p < bInfo.power; p++) {
			log("down");
			if (judgeBuilding(Vec2(bInfo.position.x, bInfo.position.y - 40 * p))) return;
			boomInSameTime(bInfo.tileX, bInfo.tileY + p);
			if (judgeReBoom(Vec2(bInfo.position.x, bInfo.position.y - 40 * p))) return;
			judgeBoomHero(player1, bInfo.tileX, bInfo.tileY +p);
			if (player2 != NULL) judgeBoomHero(player2, bInfo.tileX, bInfo.tileY+ p);
			Sprite * temp;
			temp = Sprite::createWithSpriteFrameName("down1.png");
			temp->setPosition(Vec2(bInfo.position.x, bInfo.position.y - 40 * p));
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
	log("down");
	if (judgeBuilding(Vec2(bInfo.position.x, bInfo.position.y - 40 * bInfo.power))) return;
	boomInSameTime(bInfo.tileX, bInfo.tileY + bInfo.power);
	judgeBoomHero(player1, bInfo.tileX, bInfo.tileY+ bInfo.power);
	if (player2 != NULL) judgeBoomHero(player2, bInfo.tileX, bInfo.tileY+ bInfo.power);
	Sprite * bubble;
	bubble = Sprite::createWithSpriteFrameName("downSpout1.png");
	bubble->setPosition(Vec2(bInfo.position.x, bInfo.position.y - 40 * bInfo.power));
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

void Bubble::up(bubbleInformation bInfo) {
	if (bInfo.power > 1) {
		for (int p = 1; p < bInfo.power; p++) {
			log("up");
			if (judgeBuilding(Vec2(bInfo.position.x, bInfo.position.y + 40 * p))) return;
			boomInSameTime(bInfo.tileX, bInfo.tileY - p);
			if (judgeReBoom(Vec2(bInfo.position.x, bInfo.position.y + 40 * p))) return;
			judgeBoomHero(player1, bInfo.tileX, bInfo.tileY-p);
			if (player2 != NULL) judgeBoomHero(player2, bInfo.tileX, bInfo.tileY- p);
			Sprite * temp;
			temp = Sprite::createWithSpriteFrameName("up1.png");
			temp->setPosition(Vec2(bInfo.position.x, bInfo.position.y + 40 * p));
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
	log("up");
	if (judgeBuilding(Vec2(bInfo.position.x, bInfo.position.y + 40 * bInfo.power))) return;
	judgeBoomHero(player1, bInfo.tileX, bInfo.tileY- bInfo.power);
	if (player2 != NULL) judgeBoomHero(player2, bInfo.tileX, bInfo.tileY- bInfo.power);
	boomInSameTime(bInfo.tileX, bInfo.tileY - bInfo.power);
	Sprite * bubble;	
	bubble = Sprite::createWithSpriteFrameName("upSpout1.png");
	bubble->setPosition(Vec2(bInfo.position.x, bInfo.position.y + 40 * bInfo.power));
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

void Bubble::left(bubbleInformation bInfo) {
	if (bInfo.power > 1) {
		for (int p = 1; p < bInfo.power; p++) {
			if (judgeBuilding(Vec2(bInfo.position.x - 40 * p, bInfo.position.y))) return;
			boomInSameTime(bInfo.tileX - p, bInfo.tileY);
			if (judgeReBoom(Vec2(bInfo.position.x - 40 * p, bInfo.position.y))) return;
			judgeBoomHero(player1, bInfo.tileX-p, bInfo.tileY);
			if (player2 != NULL) judgeBoomHero(player2, bInfo.tileX-p, bInfo.tileY);
			Sprite * temp;
			temp = Sprite::createWithSpriteFrameName("left1.png");
			temp->setPosition(Vec2(bInfo.position.x - 40 * p, bInfo.position.y));
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
	if (judgeBuilding(Vec2(bInfo.position.x - 40 * bInfo.power, bInfo.position.y))) return;
	judgeBoomHero(player1, bInfo.tileX- bInfo.power, bInfo.tileY);
	if (player2 != NULL) judgeBoomHero(player2, bInfo.tileX- bInfo.power, bInfo.tileY);
	boomInSameTime(bInfo.tileX- bInfo.power, bInfo.tileY);
	Sprite *bubble;
	bubble = Sprite::createWithSpriteFrameName("leftSpout1.png");
	bubble->setPosition(Vec2(bInfo.position.x - 40 * bInfo.power, bInfo.position.y));
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

void Bubble::right(bubbleInformation bInfo) {
	if (bInfo.power > 1) {
		for (int p = 1; p < bInfo.power; p++) {
			if (judgeBuilding(Vec2(bInfo.position.x + 40 * p, bInfo.position.y))) return;
			boomInSameTime(bInfo.tileX + p, bInfo.tileY);
			if (judgeReBoom(Vec2(bInfo.position.x + 40 * p, bInfo.position.y))) return;
			judgeBoomHero(player1, bInfo.tileX+p, bInfo.tileY);
			if (player2 != NULL) judgeBoomHero(player2, bInfo.tileX+p, bInfo.tileY);
			Sprite * temp;
			temp = Sprite::createWithSpriteFrameName("right1.png");
			temp->setPosition(Vec2(bInfo.position.x + 40 * p, bInfo.position.y));
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
	if (judgeBuilding(Vec2(bInfo.position.x + 40 * bInfo.power, bInfo.position.y))) return;
	judgeBoomHero(player1, bInfo.tileX+ bInfo.power, bInfo.tileY);
	if (player2 != NULL) judgeBoomHero(player2, bInfo.tileX+ bInfo.power, bInfo.tileY);
	boomInSameTime(bInfo.tileX+ bInfo.power, bInfo.tileY);
	Sprite * bubble;
	bubble = Sprite::createWithSpriteFrameName("rightSpout1.png");
	bubble->setPosition(Vec2(bInfo.position.x + 40 * bInfo.power, bInfo.position.y));
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

