#include "Bubble.h"

USING_NS_CC;


Bubble* Bubble::create(const std::string& spriteFrameName)
{
	Bubble *bubble = new Bubble();

	if (bubble && bubble->initWithSpriteFrameName(spriteFrameName)) {
		bubble->autorelease();

		return bubble;
	}

	CC_SAFE_DELETE(bubble);

	return NULL;
}

void Bubble::setPointer(TMXLayer* Building,TMXTiledMap* Map) {//标记人物所在场景
	building = Building;
	map = Map;
}

Vec2 Bubble::tileCoordFromPosition(Vec2 position)//参数是人物在整个场景中的坐标,得到人物锚点所在的瓦片坐标
{
	int x = (position.x - 20) / map->getTileSize().width;//正确吗？
	int y = (map->getMapSize().height*map->getTileSize().height - position.y + 40)
		/ map->getTileSize().height;	//没有考虑锚点
	return Vec2(x, y);
}

void Bubble::eraseFront() {//弹出最前面的两个
	if (!xQueue.empty()) {
		xQueue.pop();
		yQueue.pop();
		xVector.erase(xVector.begin());
		yVector.erase(yVector.begin());
	}
}


Vec2 Bubble::getPlacePosition(Vec2 position) {//得到人物坐标，输出泡泡坐标
	Vec2 post = tileCoordFromPosition(position);
	xVector.push_back(post.x);
	yVector.push_back(post.y);
	float x = post.x*map->getTileSize().width + map->getTileSize().width / 2 + 20 + this->getContentSize().width / 2;
	float y= (map->getMapSize().height - post.y -1)*map->getTileSize().height +//为什么？
		map->getTileSize().height/2 + 40 + this->getContentSize().height / 2-1;
	return Vec2(x, y);
}

void Bubble::innitAnimation(Animation * ani, int n, const char s[]) {//成功优化，美滋滋,大幅减小代码数量
	for (int i = 1; i <= n; i++)
	{
		__String *FrameName = __String::createWithFormat("%s%d.png",s,i);
		SpriteFrame *bubbleFrame =
			SpriteFrameCache::getInstance()->getSpriteFrameByName(FrameName->getCString());
		ani->addSpriteFrame(bubbleFrame);
	}
}


//判断建筑物，并进行消除。同时判断是否出了瓦片地图
bool Bubble::judgeBuilding(Vec2 pos) {
	Vec2 tileCoord = tileCoordFromPosition(pos);
	--tileCoord.x;//为什么？
	int x = tileCoord.x;
	int y = tileCoord.y;
	if (x<0||y<0||x>=15||y>=13) return true;//还有问题
	int tileGid = building->getTileGIDAt(tileCoord);
	if (tileGid) {//如果遇到可被炸毁的建筑物
		building->removeTileAt(tileCoord);
		return true;
	}
	return false;
}

//放置泡泡
void Bubble::placeBubble(Vec2 p,Hero * hero) {//得到的坐标是人物坐标
	if (hero->bubbleNumber == hero->placeBubbleNumber) return;
	else {
		owner = hero;
		hero->placeBubbleNumber++;
		Sprite *bubble;
		bubble = Sprite::createWithSpriteFrameName("bubble1.png");
		bubble->setAnchorPoint(Vec2(0.5f, 0.5f));

		Vec2 position = getPlacePosition(p);
		xQueue.push(position.x);
		yQueue.push(position.y);
		bubble->setPosition(position);//这个位置和初试泡泡的位置有关
		this->addChild(bubble);
		
		Animation *bubbleAnimation = Animation::create();//设置初试泡泡跳动动画
		innitAnimation(bubbleAnimation, 3, "bubble");
		bubbleAnimation->setDelayPerUnit(0.15f);//两帧之间延迟
		Animate *bubbleAnimate = Animate::create(bubbleAnimation);
		
		auto bubbleBoomFunc = CallFunc::create(CC_CALLBACK_0(Bubble::bubbleBoom, this, hero->bubblePower));//创建泡泡爆炸回调函数

		auto bubbleAction = Sequence::create(Repeat::create(bubbleAnimate, 5),bubbleBoomFunc,//记得把重复次数修改为6
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)),NULL);
		bubble->runAction(bubbleAction);//通过removeFromParent消除了残留图片
		}
}

void Bubble::bubbleBoom(int power) {
	firstEnter = true;
	Sprite * bubble;
	bubble = Sprite::createWithSpriteFrameName("bubbleCenter1.png");
	bubble->setPosition(Vec2(xQueue.front(), yQueue.front()));
	this->addChild(bubble);
	Animation *bubbleCenterAnimation = Animation::create();
	innitAnimation(bubbleCenterAnimation, 4, "bubbleCenter");
	bubbleCenterAnimation->setDelayPerUnit(0.05f);//两帧之间延迟
	Animate *bubbleCenterAnimate = Animate::create(bubbleCenterAnimation);//创建泡泡中间爆炸动画

	auto upAction = CallFunc::create(CC_CALLBACK_0(Bubble::up, this, power));
	auto rightAction = CallFunc::create(CC_CALLBACK_0(Bubble::right, this, power));
	auto leftAction = CallFunc::create(CC_CALLBACK_0(Bubble::left, this, power));
	auto downAction = CallFunc::create(CC_CALLBACK_0(Bubble::down, this, power));
	auto spawnAction = Spawn::create(bubbleCenterAnimate,downAction,upAction,
		leftAction,rightAction,NULL);
	auto delayTime = DelayTime::create(0.001f);
	auto action = Sequence::create(spawnAction,delayTime, 
		CallFunc::create(CC_CALLBACK_0(Bubble::eraseFront, this)),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent,bubble)),NULL);
	bubble->runAction(action);
}


void Bubble::down(int power) {
	if (firstEnter) {//第一次调用，记录爆炸威力
		bubblePower = power;
		firstEnter = false;
	}
	if (power == 1) {
		owner->placeBubbleNumber--;//动作完成后，放炮数-1
		if (judgeBuilding(Vec2(xQueue.front(), yQueue.front() - 40 * bubblePower))) return;
		Sprite * bubble;
		bubble = Sprite::createWithSpriteFrameName("downSpout1.png");
		bubble->setPosition(Vec2(xQueue.front(), yQueue.front() - 40 * bubblePower));
		this->addChild(bubble);
		Animation *bubbleAnimation = Animation::create();
		innitAnimation(bubbleAnimation, 12, "downSpout");
		bubbleAnimation->setDelayPerUnit(0.02f);//两帧之间延迟
		Animate *bubbleAnimate = Animate::create(bubbleAnimation);
		auto delayTime = DelayTime::create(0.005f);
		auto action = Sequence::create(bubbleAnimate, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)), NULL);
		bubble->runAction(action);
		return;
	}
	else {
		if (judgeBuilding(Vec2(xQueue.front(), yQueue.front() - 40 * (power - 1)))) {
			owner->placeBubbleNumber--;
			return;
		}
		Sprite * temp;
		temp = Sprite::createWithSpriteFrameName("down1.png");
		temp->setPosition(Vec2(xQueue.front(),yQueue.front()-40*(power-1)));
		this->addChild(temp);
		Animation *tempAnimation = Animation::create();
		innitAnimation(tempAnimation, 2, "down");
		tempAnimation->setDelayPerUnit(0.02f);
		Animate *tempAnimate = Animate::create(tempAnimation);
		auto action= CallFunc::create(CC_CALLBACK_0(Bubble::down, this, power-1));
		auto tempAction = Spawn::create(Repeat::create(tempAnimate,5), action,NULL);
		auto delayTime = DelayTime::create(0.005f);
		auto bubbleAction = Sequence::create(tempAction, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)),NULL);
		temp->runAction(bubbleAction);
	}
}

void Bubble::up(int power) {
	if (power == 1) {
		if (judgeBuilding(Vec2(xQueue.front(), yQueue.front() + 40 * bubblePower))) return;
		Sprite * bubble;	
		bubble = Sprite::createWithSpriteFrameName("upSpout1.png");
		bubble->setPosition(Vec2(xQueue.front(), yQueue.front() + 40 * bubblePower));
		this->addChild(bubble);
		Animation *bubbleAnimation = Animation::create();
		innitAnimation(bubbleAnimation, 12, "upSpout");
		bubbleAnimation->setDelayPerUnit(0.02f);//两帧之间延迟
		Animate *bubbleAnimate = Animate::create(bubbleAnimation);
		auto delayTime = DelayTime::create(0.005f);
		auto action = Sequence::create(bubbleAnimate, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)), NULL);
		bubble->runAction(action);
		return;
	}
	else {
		if (judgeBuilding(Vec2(xQueue.front(), yQueue.front() + 40 * (power - 1)))) return;
		Sprite * temp;
		temp = Sprite::createWithSpriteFrameName("up1.png");
		temp->setPosition(Vec2(xQueue.front(), yQueue.front() + 40 * (power - 1)));
		this->addChild(temp);
		Animation *tempAnimation = Animation::create();
		innitAnimation(tempAnimation, 2, "up");
		tempAnimation->setDelayPerUnit(0.02f);
		Animate *tempAnimate = Animate::create(tempAnimation);
		auto action = CallFunc::create(CC_CALLBACK_0(Bubble::up, this, power-1));
		auto tempAction = Spawn::create(Repeat::create(tempAnimate, 5), action,NULL);
		auto delayTime = DelayTime::create(0.005f);
		auto bubbleAction = Sequence::create(tempAction, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)), NULL);
		temp->runAction(bubbleAction);
	}
}

void Bubble::left(int power) {
	if (power == 1) {
		if (judgeBuilding(Vec2(xQueue.front() - 40 * bubblePower, yQueue.front()))) return;
		Sprite *bubble;
		bubble = Sprite::createWithSpriteFrameName("leftSpout1.png");
		bubble->setPosition(Vec2(xQueue.front() - 40 * bubblePower, yQueue.front()));
		this->addChild(bubble);
		Animation *bubbleLeftAnimation = Animation::create();
		innitAnimation(bubbleLeftAnimation, 12, "leftSpout");
		bubbleLeftAnimation->setDelayPerUnit(0.02f);//两帧之间延迟
		Animate *bubbleAnimate = Animate::create(bubbleLeftAnimation);
		auto delayTime = DelayTime::create(0.005f);
		auto action = Sequence::create(bubbleAnimate, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)), NULL);
		bubble->runAction(action);
		return;
	}
	else {
		if (judgeBuilding(Vec2(xQueue.front() - 40 * (power - 1), yQueue.front()))) return;
		Sprite * temp;
		temp = Sprite::createWithSpriteFrameName("left1.png");
		temp->setPosition(Vec2(xQueue.front() - 40 * (power - 1), yQueue.front()));
		this->addChild(temp);
		Animation *tempAnimation = Animation::create();
		innitAnimation(tempAnimation, 2, "left");
		tempAnimation->setDelayPerUnit(0.02f);
		Animate *tempAnimate = Animate::create(tempAnimation);
		auto action = CallFunc::create(CC_CALLBACK_0(Bubble::left, this, power-1));
		auto tempAction = Spawn::create(Repeat::create(tempAnimate, 5), action,NULL);
		auto delayTime = DelayTime::create(0.005f);
		auto bubbleAction = Sequence::create(tempAction, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)), NULL);
		temp->runAction(bubbleAction);
	}
}

void Bubble::right(int power) {
	if (power == 1) {
		if (judgeBuilding(Vec2(xQueue.front() + 40 * bubblePower, yQueue.front()))) return;
		Sprite * bubble;
		bubble = Sprite::createWithSpriteFrameName("rightSpout1.png");
		bubble->setPosition(Vec2(xQueue.front() + 40 * bubblePower, yQueue.front()));
		this->addChild(bubble);
		Animation *bubbleAnimation = Animation::create();
		innitAnimation(bubbleAnimation, 12, "rightSpout");
		bubbleAnimation->setDelayPerUnit(0.02f);//两帧之间延迟
		Animate *bubbleAnimate = Animate::create(bubbleAnimation);
		auto delayTime = DelayTime::create(0.005f);
		auto action = Sequence::create(bubbleAnimate, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)),NULL);
		bubble->runAction(action);
		return;
	}
	else {
		if (judgeBuilding(Vec2(xQueue.front() + 40 * (power - 1), yQueue.front()))) return;
		Sprite * temp;
		temp = Sprite::createWithSpriteFrameName("right1.png");
		temp->setPosition(Vec2(xQueue.front() +40 * (power - 1),yQueue.front()));
		this->addChild(temp);
		Animation *tempAnimation = Animation::create();
		innitAnimation(tempAnimation, 2, "right");
		tempAnimation->setDelayPerUnit(0.02f);
		Animate *tempAnimate = Animate::create(tempAnimation);
		auto action = CallFunc::create(CC_CALLBACK_0(Bubble::right, this, power-1));
		auto tempAction = Spawn::create(Repeat::create(tempAnimate, 5), action,NULL);
		auto delayTime = DelayTime::create(0.005f);
		auto bubbleAction = Sequence::create(tempAction, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)),NULL);
		temp->runAction(bubbleAction);
	}
}
