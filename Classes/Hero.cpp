#include "Hero.h"

USING_NS_CC;

void Hero::innitAnimation(Animation * ani, int n, const char s[]) {//成功优化，美滋滋,大幅减小代码数量
	for (int i = 1; i <= n; i++)
	{
		__String *FrameName = __String::createWithFormat("%s%d.png", s, i);
		SpriteFrame *bubbleFrame =
			SpriteFrameCache::getInstance()->getSpriteFrameByName(FrameName->getCString());
		ani->addSpriteFrame(bubbleFrame);
	}
}


Vec2 Hero::tileCoordFromPosition(Vec2 position)//拿到的是人物在整个场景中的坐标,输出瓦片坐标
{
	int x = (position.x - 20) / map->getTileSize().width;
	int y = ((map->getMapSize().height*map->getTileSize().height) - position.y + 40)
		/ map->getTileSize().height;
	return Vec2(x, y);
}

void Hero::setScene(TMXLayer* Building,TMXTiledMap* Map) {
	building = Building;
	map = Map;
	shoseLayer = map->getLayer("speed");
	powerLayer = map->getLayer("bubblePower");
	numLayer = map->getLayer("bubbleNum");
	propLayer = map->getLayer("prop");
}


Hero::Hero(int playerNo, int power, int speed, int number)
{
	this->playerNo = playerNo;
	bubblePower = power;
	movingSpeed=speed;
	bubbleNumber = number;
	placeBubbleNumber = 0;
	trapped = false;
	die = false;
	animationPlaying = false;
}

void Hero::toDie() {
	die = true;
}

//创建人物
Hero* Hero::create(const std::string& spriteFrameName, int playerNo)
{
	Hero *hero = new Hero(playerNo);
	if (hero && hero->initWithSpriteFrameName(spriteFrameName)) {
		hero->autorelease();

		return hero;
	}

	CC_SAFE_DELETE(hero);

	return NULL;
}

//设置位置
void Hero::setPosition(const Vec2 &position)
{
	Size screenSize = Director::getInstance()->getVisibleSize();

	float pos_x = position.x;
	float pos_y = position.y;

	if (pos_x < 20 + 20) {
		pos_x = 20 + 20;
	}
	else if (pos_x >620 - 20) {
		pos_x = 620 - 20;
	}

	if (pos_y < 40 + 6) {
		pos_y = 40 + 6;
	}
	else if (pos_y >560 - 34) {
		pos_y = 560 - 34;
	}


	heroPosition = Vec2(pos_x, pos_y);
	Sprite::setPosition(Vec2(pos_x, pos_y));
	Sprite::setAnchorPoint(Vec2(0.5f, 0.1f));//人物锚点需要改进，边界问题
}

//人物移动
void Hero::moveHero(const EventKeyboard::KeyCode keyCode)
{
	Vec2 centerPos;
	Vec2 position = heroPosition;
	centerPos.x = heroPosition.x;
	centerPos.y = heroPosition.y + 14;

	Vec2 collisionPos1; //允许1像素的优化
	Vec2 collisionPos2;
	Vec2 collisionCenter;
	Vec2 tileCoord1;
	Vec2 tileCoord2;
	Vec2 centerCoord;
	int tileGid1;
	int tileGid2;
	int centerGid;


	if (playerNo == 1)
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			position.x -= movingSpeed;
			judgeOnProps(centerPos);
			if (!animationPlaying)
			{
				Animation *moveLeftAnimation = Animation::create();
				innitAnimation(moveLeftAnimation, 4, "hero1Left");
				moveLeftAnimation->setDelayPerUnit(0.1f);
				Animate *moveLeftAnimate = Animate::create(moveLeftAnimation);
				runAction(RepeatForever::create(moveLeftAnimate));
			}
			centerPos.x -= movingSpeed;
			if (centerPos.x <= 20 + 20)
			{
				position.x = 20 + tileCoordFromPosition(heroPosition).x * 40 + 20;
				break;
			}
			collisionPos1.x = centerPos.x - 20;
			collisionPos1.y = centerPos.y + 20 - 1;
			collisionPos2.x = centerPos.x - 20;
			collisionPos2.y = centerPos.y - 20 + 1;
			tileCoord1 = tileCoordFromPosition(collisionPos1);
			tileCoord2 = tileCoordFromPosition(collisionPos2);
			tileGid1 = building->getTileGIDAt(tileCoord1);
			tileGid2 = building->getTileGIDAt(tileCoord2);

			if (tileGid1 || tileGid2)
			{
				collisionCenter.x = (collisionPos1.x + collisionPos2.x) / 2;
				collisionCenter.y = (collisionPos1.y + collisionPos2.y) / 2;
				centerCoord = tileCoordFromPosition(collisionCenter);
				centerGid = building->getTileGIDAt(centerCoord);
				if (centerGid == 0)
				{
					if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 > 20)
					{
						position.y += movingSpeed;
						centerPos.y += movingSpeed;
						if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 < 20)
						{
							position.y = 560 - tileCoordFromPosition(centerPos).y * 40 - 34;
						}
					}
					else
					{
						position.y -= movingSpeed;
						centerPos.y -= movingSpeed;
						if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 > 20)
						{
							position.y = 560 - tileCoordFromPosition(centerPos).y * 40 - 34;
						}
					}
				}
				position.x = 20 + tileCoordFromPosition(heroPosition).x * 40 + 20;

			}
			break;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:

			position.x += movingSpeed;
			judgeOnProps(centerPos);
			if (!animationPlaying)
			{
				Animation *moveRightAnimation = Animation::create();
				innitAnimation(moveRightAnimation, 4, "hero1Right");
				moveRightAnimation->setDelayPerUnit(0.1f);
				Animate *moveRightAnimate = Animate::create(moveRightAnimation);
				runAction(RepeatForever::create(moveRightAnimate));
			}

			centerPos.x += movingSpeed;
			if (centerPos.x >= 620 - 20)
			{
				position.x = 20 + tileCoordFromPosition(heroPosition).x * 40 + 20;
				break;
			}
			collisionPos1.x = centerPos.x + 20;
			collisionPos1.y = centerPos.y + 20 - 1;
			collisionPos2.x = centerPos.x + 20;
			collisionPos2.y = centerPos.y - 20 + 1;
			tileCoord1 = tileCoordFromPosition(collisionPos1);
			tileCoord2 = tileCoordFromPosition(collisionPos2);
			tileGid1 = building->getTileGIDAt(tileCoord1);
			tileGid2 = building->getTileGIDAt(tileCoord2);

			if (tileGid1 || tileGid2)
			{
				collisionCenter.x = (collisionPos1.x + collisionPos2.x) / 2;
				collisionCenter.y = (collisionPos1.y + collisionPos2.y) / 2;
				centerCoord = tileCoordFromPosition(collisionCenter);
				centerGid = building->getTileGIDAt(centerCoord);
				if (centerGid == 0)
				{
					if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 > 20)
					{
						position.y += movingSpeed;
						centerPos.y += movingSpeed;
						if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 < 20)
						{
							position.y = 560 - tileCoordFromPosition(centerPos).y * 40 - 34;
						}
					}
					else
					{
						position.y -= movingSpeed;
						centerPos.y -= movingSpeed;
						if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 > 20)
						{
							position.y = 560 - tileCoordFromPosition(centerPos).y * 40 - 34;
						}
					}
				}
				position.x = 20 + tileCoordFromPosition(heroPosition).x * 40 + 20;

			}
			break;

		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			position.y -= movingSpeed;
			judgeOnProps(centerPos);
			if (!animationPlaying)
			{
				Animation *moveDownAnimation = Animation::create();
				innitAnimation(moveDownAnimation, 4, "hero1Down");
				moveDownAnimation->setDelayPerUnit(0.1f);
				Animate *moveDownAnimate = Animate::create(moveDownAnimation);
				runAction(RepeatForever::create(moveDownAnimate));
			}
			centerPos.y -= movingSpeed;
			if (centerPos.y <= 40 + 20)
			{
				position.y = 560 - tileCoordFromPosition(heroPosition).y * 40 - 34;
				break;
			}
			collisionPos1.x = centerPos.x + 20 - 1;
			collisionPos1.y = centerPos.y - 20;
			collisionPos2.x = centerPos.x - 20 + 1;
			collisionPos2.y = centerPos.y - 20;
			tileCoord1 = tileCoordFromPosition(collisionPos1);
			tileCoord2 = tileCoordFromPosition(collisionPos2);
			tileGid1 = building->getTileGIDAt(tileCoord1);
			tileGid2 = building->getTileGIDAt(tileCoord2);

			if (tileGid1 || tileGid2)
			{
				collisionCenter.x = (collisionPos1.x + collisionPos2.x) / 2;
				collisionCenter.y = (collisionPos1.y + collisionPos2.y) / 2;
				centerCoord = tileCoordFromPosition(collisionCenter);
				centerGid = building->getTileGIDAt(centerCoord);
				if (centerGid == 0)
				{
					if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 > 20)
					{
						position.x -= movingSpeed;
						centerPos.x -= movingSpeed;
						if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 < 20)
						{
							position.x = 20 + tileCoordFromPosition(centerPos).x * 40 + 20;
						}
					}
					else
					{
						position.x += movingSpeed;
						centerPos.x += movingSpeed;
						if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 > 20)
						{
							position.x = 20 + tileCoordFromPosition(centerPos).x * 40 + 20;
						}
					}
				}
				position.y = 560 - tileCoordFromPosition(heroPosition).y * 40 - 34;
			}
			break;

		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			position.y += movingSpeed;
			judgeOnProps(centerPos);
			if (!animationPlaying)
			{
				Animation *moveUpAnimation = Animation::create();
				innitAnimation(moveUpAnimation, 4, "hero1Up");
				moveUpAnimation->setDelayPerUnit(0.1f);
				moveUpAnimation->setRestoreOriginalFrame(true);
				Animate *moveUpAnimate = Animate::create(moveUpAnimation);
				runAction(RepeatForever::create(moveUpAnimate));
			}
			centerPos.y += movingSpeed;
			if (centerPos.y >= 560 - 20)
			{
				position.y = 560 - tileCoordFromPosition(heroPosition).y * 40 - 34;
				break;
			}
			collisionPos1.x = centerPos.x + 20 - 1;
			collisionPos1.y = centerPos.y + 20;
			collisionPos2.x = centerPos.x - 20 + 1;
			collisionPos2.y = centerPos.y + 20;
			tileCoord1 = tileCoordFromPosition(collisionPos1);
			tileCoord2 = tileCoordFromPosition(collisionPos2);
			tileGid1 = building->getTileGIDAt(tileCoord1);
			tileGid2 = building->getTileGIDAt(tileCoord2);

			if (tileGid1 || tileGid2)
			{
				collisionCenter.x = (collisionPos1.x + collisionPos2.x) / 2;
				collisionCenter.y = (collisionPos1.y + collisionPos2.y) / 2;
				centerCoord = tileCoordFromPosition(collisionCenter);
				centerGid = building->getTileGIDAt(centerCoord);
				if (centerGid == 0)
				{
					if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 > 20)
					{
						position.x -= movingSpeed;
						centerPos.x -= movingSpeed;
						if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 < 20)
						{
							position.x = 20 + tileCoordFromPosition(centerPos).x * 40 + 20;
						}
					}
					else
					{
						position.x += movingSpeed;
						centerPos.x += movingSpeed;
						if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 > 20)
						{
							position.x = 20 + tileCoordFromPosition(centerPos).x * 40 + 20;
						}
					}
				}
				position.y = 560 - tileCoordFromPosition(heroPosition).y * 40 - 34;
			}
			break;

		default:
			break;
		}


		this->setPosition(position);
	}

	else if (playerNo == 2)//wsad控制移动
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_A:
			position.x -= movingSpeed;
			judgeOnProps(centerPos);
			if (!animationPlaying)
			{
				Animation *moveLeftAnimation = Animation::create();
				innitAnimation(moveLeftAnimation, 4, "hero2Left");
				moveLeftAnimation->setDelayPerUnit(0.1f);
				Animate *moveLeftAnimate = Animate::create(moveLeftAnimation);
				runAction(RepeatForever::create(moveLeftAnimate));
			}
			centerPos.x -= movingSpeed;
			if (centerPos.x <= 20 + 20)
			{
				position.x = 20 + tileCoordFromPosition(heroPosition).x * 40 + 20;
				break;
			}
			collisionPos1.x = centerPos.x - 20;
			collisionPos1.y = centerPos.y + 20 - 1;
			collisionPos2.x = centerPos.x - 20;
			collisionPos2.y = centerPos.y - 20 + 1;
			tileCoord1 = tileCoordFromPosition(collisionPos1);
			tileCoord2 = tileCoordFromPosition(collisionPos2);
			tileGid1 = building->getTileGIDAt(tileCoord1);
			tileGid2 = building->getTileGIDAt(tileCoord2);

			if (tileGid1 || tileGid2)
			{
				collisionCenter.x = (collisionPos1.x + collisionPos2.x) / 2;
				collisionCenter.y = (collisionPos1.y + collisionPos2.y) / 2;
				centerCoord = tileCoordFromPosition(collisionCenter);
				centerGid = building->getTileGIDAt(centerCoord);
				if (centerGid == 0)
				{
					if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 > 20)
					{
						position.y += movingSpeed;
						centerPos.y += movingSpeed;
						if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 < 20)
						{
							position.y = 560 - tileCoordFromPosition(centerPos).y * 40 - 34;
						}
					}
					else
					{
						position.y -= movingSpeed;
						centerPos.y -= movingSpeed;
						if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 > 20)
						{
							position.y = 560 - tileCoordFromPosition(centerPos).y * 40 - 34;
						}
					}
				}
				position.x = 20 + tileCoordFromPosition(heroPosition).x * 40 + 20;

			}
			break;

		case EventKeyboard::KeyCode::KEY_D:

			position.x += movingSpeed;
			judgeOnProps(centerPos);
			if (!animationPlaying)
			{
				Animation *moveRightAnimation = Animation::create();
				innitAnimation(moveRightAnimation, 4, "hero2Right");
				moveRightAnimation->setDelayPerUnit(0.1f);
				Animate *moveRightAnimate = Animate::create(moveRightAnimation);
				runAction(RepeatForever::create(moveRightAnimate));
			}

			centerPos.x += movingSpeed;
			if (centerPos.x >= 620 - 20)
			{
				position.x = 20 + tileCoordFromPosition(heroPosition).x * 40 + 20;
				break;
			}
			collisionPos1.x = centerPos.x + 20;
			collisionPos1.y = centerPos.y + 20 - 1;
			collisionPos2.x = centerPos.x + 20;
			collisionPos2.y = centerPos.y - 20 + 1;
			tileCoord1 = tileCoordFromPosition(collisionPos1);
			tileCoord2 = tileCoordFromPosition(collisionPos2);
			tileGid1 = building->getTileGIDAt(tileCoord1);
			tileGid2 = building->getTileGIDAt(tileCoord2);

			if (tileGid1 || tileGid2)
			{
				collisionCenter.x = (collisionPos1.x + collisionPos2.x) / 2;
				collisionCenter.y = (collisionPos1.y + collisionPos2.y) / 2;
				centerCoord = tileCoordFromPosition(collisionCenter);
				centerGid = building->getTileGIDAt(centerCoord);
				if (centerGid == 0)
				{
					if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 > 20)
					{
						position.y += movingSpeed;
						centerPos.y += movingSpeed;
						if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 < 20)
						{
							position.y = 560 - tileCoordFromPosition(centerPos).y * 40 - 34;
						}
					}
					else
					{
						position.y -= movingSpeed;
						centerPos.y -= movingSpeed;
						if (560 - centerPos.y - tileCoordFromPosition(centerPos).y * 40 > 20)
						{
							position.y = 560 - tileCoordFromPosition(centerPos).y * 40 - 34;
						}
					}
				}
				position.x = 20 + tileCoordFromPosition(heroPosition).x * 40 + 20;

			}
			break;

		case EventKeyboard::KeyCode::KEY_S:
			position.y -= movingSpeed;
			judgeOnProps(centerPos);
			if (!animationPlaying)
			{
				Animation *moveDownAnimation = Animation::create();
				innitAnimation(moveDownAnimation, 4, "hero2Down");
				moveDownAnimation->setDelayPerUnit(0.1f);
				Animate *moveDownAnimate = Animate::create(moveDownAnimation);
				runAction(RepeatForever::create(moveDownAnimate));
			}
			centerPos.y -= movingSpeed;
			if (centerPos.y <= 40 + 20)
			{
				position.y = 560 - tileCoordFromPosition(heroPosition).y * 40 - 34;
				break;
			}
			collisionPos1.x = centerPos.x + 20 - 1;
			collisionPos1.y = centerPos.y - 20;
			collisionPos2.x = centerPos.x - 20 + 1;
			collisionPos2.y = centerPos.y - 20;
			tileCoord1 = tileCoordFromPosition(collisionPos1);
			tileCoord2 = tileCoordFromPosition(collisionPos2);
			tileGid1 = building->getTileGIDAt(tileCoord1);
			tileGid2 = building->getTileGIDAt(tileCoord2);

			if (tileGid1 || tileGid2)
			{
				collisionCenter.x = (collisionPos1.x + collisionPos2.x) / 2;
				collisionCenter.y = (collisionPos1.y + collisionPos2.y) / 2;
				centerCoord = tileCoordFromPosition(collisionCenter);
				centerGid = building->getTileGIDAt(centerCoord);
				if (centerGid == 0)
				{
					if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 > 20)
					{
						position.x -= movingSpeed;
						centerPos.x -= movingSpeed;
						if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 < 20)
						{
							position.x = 20 + tileCoordFromPosition(centerPos).x * 40 + 20;
						}
					}
					else
					{
						position.x += movingSpeed;
						centerPos.x += movingSpeed;
						if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 > 20)
						{
							position.x = 20 + tileCoordFromPosition(centerPos).x * 40 + 20;
						}
					}
				}
				position.y = 560 - tileCoordFromPosition(heroPosition).y * 40 - 34;
			}
			break;

		case EventKeyboard::KeyCode::KEY_W:
			position.y += movingSpeed;
			judgeOnProps(centerPos);
			if (!animationPlaying)
			{
				Animation *moveUpAnimation = Animation::create();
				innitAnimation(moveUpAnimation, 4, "hero2Up");
				moveUpAnimation->setDelayPerUnit(0.1f);
				moveUpAnimation->setRestoreOriginalFrame(true);
				Animate *moveUpAnimate = Animate::create(moveUpAnimation);
				runAction(RepeatForever::create(moveUpAnimate));
			}
			centerPos.y += movingSpeed;
			if (centerPos.y >= 560 - 20)
			{
				position.y = 560 - tileCoordFromPosition(heroPosition).y * 40 - 34;
				break;
			}
			collisionPos1.x = centerPos.x + 20 - 1;
			collisionPos1.y = centerPos.y + 20;
			collisionPos2.x = centerPos.x - 20 + 1;
			collisionPos2.y = centerPos.y + 20;
			tileCoord1 = tileCoordFromPosition(collisionPos1);
			tileCoord2 = tileCoordFromPosition(collisionPos2);
			tileGid1 = building->getTileGIDAt(tileCoord1);
			tileGid2 = building->getTileGIDAt(tileCoord2);

			if (tileGid1 || tileGid2)
			{
				collisionCenter.x = (collisionPos1.x + collisionPos2.x) / 2;
				collisionCenter.y = (collisionPos1.y + collisionPos2.y) / 2;
				centerCoord = tileCoordFromPosition(collisionCenter);
				centerGid = building->getTileGIDAt(centerCoord);
				if (centerGid == 0)
				{
					if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 > 20)
					{
						position.x -= movingSpeed;
						centerPos.x -= movingSpeed;
						if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 < 20)
						{
							position.x = 20 + tileCoordFromPosition(centerPos).x * 40 + 20;
						}
					}
					else
					{
						position.x += movingSpeed;
						centerPos.x += movingSpeed;
						if (centerPos.x - tileCoordFromPosition(centerPos).x * 40 - 20 > 20)
						{
							position.x = 20 + tileCoordFromPosition(centerPos).x * 40 + 20;
						}
					}
				}
				position.y = 560 - tileCoordFromPosition(heroPosition).y * 40 - 34;
			}
			break;

		default:
			break;
		}


		this->setPosition(position);
	}
	
}

void Hero::setFrame(const cocos2d::EventKeyboard::KeyCode keyCode)
{
	if (playerNo == 1)
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		{
			SpriteFrame *hero1Left = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1Left.png");
			setSpriteFrame(hero1Left);
		}
		break;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		{
			SpriteFrame *hero1Right = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1Right.png");
			setSpriteFrame(hero1Right);
		}
		break;

		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		{
			SpriteFrame *hero1Down = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1Down.png");
			setSpriteFrame(hero1Down);
		}
		break;

		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		{
			SpriteFrame *hero1Up = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1Up.png");
			setSpriteFrame(hero1Up);
		}
		break;

		default:
			break;
		}
	}
	
	else if (playerNo == 2)
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_A:
		{
			SpriteFrame *hero1Left = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2Left.png");
			setSpriteFrame(hero1Left);
		}
		break;

		case EventKeyboard::KeyCode::KEY_D:
		{
			SpriteFrame *hero1Right = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2Right.png");
			setSpriteFrame(hero1Right);
		}
		break;

		case EventKeyboard::KeyCode::KEY_S:
		{
			SpriteFrame *hero1Down = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2Down.png");
			setSpriteFrame(hero1Down);
		}
		break;

		case EventKeyboard::KeyCode::KEY_W:
		{
			SpriteFrame *hero1Up = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2Up.png");
			setSpriteFrame(hero1Up);
		}
		break;

		default:
			break;
		}
	}

}

//判断道具，并进行属性更改
void Hero::judgeOnProps(const Vec2 pos) {
	Vec2 tileCoord = tileCoordFromPosition(pos);
	int tileGid= propLayer->getTileGIDAt(tileCoord);
	if (tileGid) {
		Value prop = map->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();
		std::string speed = propValueMap["speed"].asString();
		std::string number = propValueMap["number"].asString();
		std::string power = propValueMap["power"].asString();
		if (speed == "true"&&movingSpeed<=5) {
			movingSpeed++;
			propLayer->removeTileAt(tileCoord);
		}
		if (power == "true"&&bubblePower<=5) {
			bubblePower++;
			propLayer->removeTileAt(tileCoord);
		}
		if (number == "true"&&bubbleNumber<=5) {
			bubbleNumber++;
			propLayer->removeTileAt(tileCoord);
		}
	}
}

void Hero::becomeDie() {
	trapped = true;
	stopAllActions();
	Animation *dyingAnimation = Animation::create();
	if(playerNo==1) innitAnimation(dyingAnimation, 4, "hero1Dying");
	else innitAnimation(dyingAnimation, 4, "hero2Dying");
	dyingAnimation->setDelayPerUnit(0.2f);
	Animate *dyingAnimate = Animate::create(dyingAnimation);
	

	Animation *deadAnimation = Animation::create();
	if(playerNo==1) innitAnimation(deadAnimation, 4, "hero1Dead");
	else innitAnimation(deadAnimation, 4, "hero2Dead");
	deadAnimation->setDelayPerUnit(0.2f);
	Animate *deadAnimate = Animate::create(deadAnimation);
	auto delayTime = DelayTime::create(0.01f);
	auto toDieFunc=CallFunc::create(CC_CALLBACK_0(Hero::toDie, this));
	auto dieAction = Sequence::create(Repeat::create(dyingAnimate, 6),delayTime,  toDieFunc,deadAnimate,NULL);
	runAction(dieAction);

}

void Hero::win() {
	trapped = true;
	stopAllActions();
	Animation *winAnimation = Animation::create();
	if(playerNo==1) innitAnimation(winAnimation, 3, "hero1Win");
	else innitAnimation(winAnimation, 3, "hero2Win");
	winAnimation->setDelayPerUnit(0.3f);
	Animate *winAnimate = Animate::create(winAnimation);
	runAction(RepeatForever::create(winAnimate));
}

Vec2 Hero::getPosition()
{
	return heroPosition;
}