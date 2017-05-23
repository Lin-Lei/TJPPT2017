#include "Bubble.h"
#include"vector"
USING_NS_CC;

std::vector<float>xVector;//��¼ÿ�η��ڵ�λ��
std::vector<float>yVector;//��ᵼ�£�vector����
int count = 0;

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

void Bubble::setPointer(TMXLayer* Building,TMXTiledMap* Map) {
	building = Building;
	map = Map;
}

Vec2 Bubble::getPlacePosition(Vec2 position) {//�õ��������꣬�����������
	Vec2 post = tileCoordFromPosition(position);
	float x= post.x*map->getTileSize().width + map->getTileSize().width / 2 + 20 + this->getContentSize().width/2;
	float y= (map->getMapSize().height - post.y -1)*map->getTileSize().height +
		map->getTileSize().height/2 + 40 + this->getContentSize().height / 2;
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

bool Bubble::judgeBuilding(Vec2 pos) {
	Vec2 tileCoord = tileCoordFromPosition(pos);
	int tileGid = building->getTileGIDAt(tileCoord);
	if (tileGid) {//��������ɱ�ը�ٵĽ�����
		building->removeTileAt(tileCoord);
		return true;
	}
	return false;
}

Vec2 Bubble::tileCoordFromPosition(Vec2 position)//�õ��������������������е�����
{
	int x = (position.x - 20) / map->getTileSize().width;
	int y = ((map->getMapSize().height*map->getTileSize().height) - position.y + 40)
		/ map->getTileSize().height;
	return Vec2(x, y);
}

//��������
void Bubble::placeBubble(Vec2 p,Hero * hero) {//�õ�����������������
	if (hero->bubbleNumber == hero->placeBubbleNumber) return;
	else {
		owner = hero;
		hero->placeBubbleNumber++;
		Sprite *bubble;
		bubble = Sprite::createWithSpriteFrameName("bubble1.png");
		bubble->setAnchorPoint(Vec2(0.5f, 0.5f));

		Vec2 position = getPlacePosition(p);
		xVector.push_back(position.x);
		yVector.push_back(position.y);
		bubble->setPosition(position);//���λ�úͳ������ݵ�λ���й�
		this->addChild(bubble);
		
		Animation *bubbleAnimation = Animation::create();//���ó���������������
		innitAnimation(bubbleAnimation, 3, "bubble");
		bubbleAnimation->setDelayPerUnit(0.15f);//��֮֡���ӳ�
		Animate *bubbleAnimate = Animate::create(bubbleAnimation);
		
		auto bubbleBoomFunc = CallFunc::create(CC_CALLBACK_0(Bubble::bubbleBoom, this, hero->bubblePower));//�������ݱ�ը�ص�����

		auto bubbleAction = Sequence::create(Repeat::create(bubbleAnimate, 6),bubbleBoomFunc,//�ǵð��ظ������޸�Ϊ6
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)),NULL);
		bubble->runAction(bubbleAction);//ͨ��removeFromParent�����˲���ͼƬ
		}
}

void Bubble::bubbleBoom(int power) {
	count++;
	Sprite * bubble;
	bubble = Sprite::createWithSpriteFrameName("bubbleCenter1.png");
	bubble->setPosition(Vec2(xVector[count-1], yVector[count-1]));
	this->addChild(bubble);
	Animation *bubbleCenterAnimation = Animation::create();
	innitAnimation(bubbleCenterAnimation, 4, "bubbleCenter");
	bubbleCenterAnimation->setDelayPerUnit(0.1f);//��֮֡���ӳ�
	Animate *bubbleCenterAnimate = Animate::create(bubbleCenterAnimation);//���������м䱬ը����

	auto upAction = CallFunc::create(CC_CALLBACK_0(Bubble::up, this, power));
	auto rightAction = CallFunc::create(CC_CALLBACK_0(Bubble::right, this, power));
	auto leftAction = CallFunc::create(CC_CALLBACK_0(Bubble::left, this, power));
	auto downAction = CallFunc::create(CC_CALLBACK_0(Bubble::down, this, power));
	auto spawnAction = Spawn::create(bubbleCenterAnimate,downAction,upAction,
		leftAction,rightAction,NULL);
	auto delayTime = DelayTime::create(0.01f);
	auto action = Sequence::create(spawnAction,delayTime,
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent,bubble)),NULL);
	bubble->runAction(action);
}


void Bubble::down(int power) {
	if (power == 1) {
		owner->placeBubbleNumber--;//������ɺ󣬷�����-1
		if (judgeBuilding(Vec2(xVector[count - 1], yVector[count - 1] - 40 * owner->bubblePower))) return;
		Sprite * bubble;
		bubble = Sprite::createWithSpriteFrameName("downSpout1.png");
		bubble->setPosition(Vec2(xVector[count - 1], yVector[count - 1] - 40 * owner->bubblePower));
		this->addChild(bubble);
		Animation *bubbleAnimation = Animation::create();
		innitAnimation(bubbleAnimation, 12, "downSpout");
		bubbleAnimation->setDelayPerUnit(0.04f);//��֮֡���ӳ�
		Animate *bubbleAnimate = Animate::create(bubbleAnimation);
		auto delayTime = DelayTime::create(0.01f);
		auto action = Sequence::create(bubbleAnimate, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)), NULL);
		bubble->runAction(action);
		return;
	}
	else {
		if (judgeBuilding(Vec2(xVector[count - 1], yVector[count - 1] - 40 * (power - 1)))) {
			owner->placeBubbleNumber--;
			return;
		}
		Sprite * temp;
		temp = Sprite::createWithSpriteFrameName("down1.png");
		temp->setPosition(Vec2(xVector[count-1],yVector[count-1]-40*(power-1)));
		this->addChild(temp);
		Animation *tempAnimation = Animation::create();
		innitAnimation(tempAnimation, 2, "down");
		tempAnimation->setDelayPerUnit(0.04f);
		Animate *tempAnimate = Animate::create(tempAnimation);
		auto action= CallFunc::create(CC_CALLBACK_0(Bubble::down, this, power-1));
		auto tempAction = Spawn::create(Repeat::create(tempAnimate,5), action,NULL);
		auto delayTime = DelayTime::create(0.01f);
		auto bubbleAction = Sequence::create(tempAction, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)), NULL);
		temp->runAction(bubbleAction);
	}
}

void Bubble::up(int power) {
	if (power == 1) {
		if (judgeBuilding(Vec2(xVector[count - 1], yVector[count - 1] + 40 * owner->bubblePower))) return;
		Sprite * bubble;
		bubble = Sprite::createWithSpriteFrameName("upSpout1.png");
		bubble->setPosition(Vec2(xVector[count - 1], yVector[count - 1] + 40 * owner->bubblePower));
		this->addChild(bubble);
		Animation *bubbleAnimation = Animation::create();
		innitAnimation(bubbleAnimation, 12, "upSpout");
		bubbleAnimation->setDelayPerUnit(0.04f);//��֮֡���ӳ�
		Animate *bubbleAnimate = Animate::create(bubbleAnimation);
		auto delayTime = DelayTime::create(0.01f);
		auto action = Sequence::create(bubbleAnimate, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)), NULL);
		bubble->runAction(action);
		return;
	}
	else {
		if (judgeBuilding(Vec2(xVector[count - 1], yVector[count - 1] + 40 * (power - 1)))) return;
		Sprite * temp;
		temp = Sprite::createWithSpriteFrameName("up1.png");
		temp->setPosition(Vec2(xVector[count-1], yVector[count-1] + 40 * (power - 1)));
		this->addChild(temp);
		Animation *tempAnimation = Animation::create();
		innitAnimation(tempAnimation, 2, "up");
		tempAnimation->setDelayPerUnit(0.04f);
		Animate *tempAnimate = Animate::create(tempAnimation);
		auto action = CallFunc::create(CC_CALLBACK_0(Bubble::up, this, power-1));
		auto tempAction = Spawn::create(Repeat::create(tempAnimate, 5), action,NULL);
		auto delayTime = DelayTime::create(0.01f);
		auto bubbleAction = Sequence::create(tempAction, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)), NULL);
		temp->runAction(bubbleAction);
	}
}

void Bubble::left(int power) {
	if (power == 1) {
		if (judgeBuilding(Vec2(xVector[count - 1] - 40 * owner->bubblePower, yVector[count - 1]))) return;
		Sprite *bubble;
		bubble = Sprite::createWithSpriteFrameName("leftSpout1.png");
		bubble->setPosition(Vec2(xVector[count - 1] - 40 * owner->bubblePower, yVector[count - 1]));
		this->addChild(bubble);
		Animation *bubbleLeftAnimation = Animation::create();
		innitAnimation(bubbleLeftAnimation, 12, "leftSpout");
		bubbleLeftAnimation->setDelayPerUnit(0.04f);//��֮֡���ӳ�
		Animate *bubbleAnimate = Animate::create(bubbleLeftAnimation);
		auto delayTime = DelayTime::create(0.01f);
		auto action = Sequence::create(bubbleAnimate, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)), NULL);
		bubble->runAction(action);
		return;
	}
	else {
		if (judgeBuilding(Vec2(xVector[count - 1] - 40 * (power - 1), yVector[count - 1]))) return;
		Sprite * temp;
		temp = Sprite::createWithSpriteFrameName("left1.png");
		temp->setPosition(Vec2(xVector[count-1] - 40 * (power - 1), yVector[count-1]));
		this->addChild(temp);
		Animation *tempAnimation = Animation::create();
		innitAnimation(tempAnimation, 2, "left");
		tempAnimation->setDelayPerUnit(0.04f);
		Animate *tempAnimate = Animate::create(tempAnimation);
		auto action = CallFunc::create(CC_CALLBACK_0(Bubble::left, this, power-1));
		auto tempAction = Spawn::create(Repeat::create(tempAnimate, 5), action,NULL);
		auto delayTime = DelayTime::create(0.01f);
		auto bubbleAction = Sequence::create(tempAction, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)), NULL);
		temp->runAction(bubbleAction);
	}
}


void Bubble::right(int power) {
	if (power == 1) {
		if (judgeBuilding(Vec2(xVector[count - 1] + 40 * owner->bubblePower, yVector[count - 1]))) return;
		Sprite * bubble;
		bubble = Sprite::createWithSpriteFrameName("rightSpout1.png");
		bubble->setPosition(Vec2(xVector[count - 1] + 40 * owner->bubblePower, yVector[count - 1]));
		this->addChild(bubble);
		Animation *bubbleAnimation = Animation::create();
		innitAnimation(bubbleAnimation, 12, "rightSpout");
		bubbleAnimation->setDelayPerUnit(0.04f);//��֮֡���ӳ�
		Animate *bubbleAnimate = Animate::create(bubbleAnimation);
		auto delayTime = DelayTime::create(0.01f);
		auto action = Sequence::create(bubbleAnimate, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, bubble)), NULL);
		bubble->runAction(action);
		return;
	}
	else {
		if (judgeBuilding(Vec2(xVector[count - 1] + 40 * (power - 1), yVector[count - 1]))) return;
		Sprite * temp;
		temp = Sprite::createWithSpriteFrameName("right1.png");
		temp->setPosition(Vec2(xVector[count-1] +40 * (power - 1),yVector[count-1]));
		this->addChild(temp);
		Animation *tempAnimation = Animation::create();
		innitAnimation(tempAnimation, 2, "right");
		tempAnimation->setDelayPerUnit(0.04f);
		Animate *tempAnimate = Animate::create(tempAnimation);
		auto action = CallFunc::create(CC_CALLBACK_0(Bubble::right, this, power-1));
		auto tempAction = Spawn::create(Repeat::create(tempAnimate, 5), action,NULL);
		auto delayTime = DelayTime::create(0.01f);
		auto bubbleAction = Sequence::create(tempAction, delayTime,
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, temp)), NULL);
		temp->runAction(bubbleAction);
	}
}
