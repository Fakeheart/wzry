#include "GameScene.h"
#include "Actor.h"
#include "Constant.h"
#include "HealthComponent.h"
#include "HRocker.h"
#include "MovingActor.h"
#include "Projectile.h"
#include <math.h>
#include <time.h>
USING_NS_CC;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	//初始化层
	_CharacterLayer = Layer::create();
	_MenuLayer = Layer::create();
	//创建按钮
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		[&](Object* sender) {
		Director::getInstance()->end();
		#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
		exit(0);
		#endif
	});
	//初始化地图
	//1360 2315 3200
	//700 700 700 700 800 700 700 700 700
	_MapLayer = Layer::create();
	this->addChild(_MapLayer, -2);
	_map = TMXTiledMap::create("map/2.tmx");
	_MapLayer->addChild(_map, 0, MAPTAG);
	_MapLayer ->setAnchorPoint(Vec2::ZERO);
	_MapLayer->setPosition(Vec2::ZERO);
	_map->setAnchorPoint(Vec2(0, 0));
	_map->setPosition(Vec2(0, -3675 + visibleSize.height / 2));
	_map->setScale(2);
	//创建菜单
	auto menu = Menu::create(closeItem, NULL, 2);
	menu->setPosition(Vec2::ZERO);
	closeItem->setPosition(Vec2(visibleSize.width-closeItem->getContentSize().width/2, closeItem->getContentSize().height / 2));
	this->addChild(menu, 1);
	// 初始化敌方小兵出生地
	_enemyBirthPlace = Vec2(5500, MAPHEIGHT / 2);
	//初始化敌方小兵
	for (int i = 0; i < 4; i++)
	{
		auto soldier = MovingActor::create("soldier.png", ECamp::BLUE);
		soldier->setPosition(_enemyBirthPlace + Vec2(150*i, 0));
		_enemySoldiers.pushBack(soldier);
		soldier->setScale(0.5);
		soldier->setTag(TAG_YUAN);
		_MapLayer->addChild(soldier);
		soldier->getHealth()->setScale(0.5);
		soldier->setMoveSpeed(200);
		
		soldier->setAttackRadius(200);
		soldier->setHostilityRadius(400);
	}
	//初始化地图里的塔
	_ourTa1 = Actor::create("map/redTa.png", ECamp::RED);
	_ourTa2 = Actor::create("map/redTa.png", ECamp::RED);
	_enemyTa1 = Actor::create("map/redTa.png", ECamp::BLUE);
	_enemyTa2 = Actor::create("map/redTa.png", ECamp::BLUE);
	
	_ourTa2->setPosition(Vec2(1400, MAPHEIGHT / 2));
	_ourTa1->setPosition(Vec2(2100, MAPHEIGHT / 2));
	_enemyTa1->setPosition(Vec2(4300 , MAPHEIGHT / 2));
	_enemyTa2->setPosition(Vec2(5000, MAPHEIGHT / 2));
	
	_ourTa2->setScale(0.4);
	_ourTa1->setScale(0.4);
	_enemyTa1->setScale(0.4);
	_enemyTa2->setScale(0.4);
	
	_MapLayer->addChild(_ourTa2);
	_MapLayer->addChild(_ourTa1);
	_MapLayer->addChild(_enemyTa1);
	_MapLayer->addChild(_enemyTa2);
	
	_ourTa2->setAttack(300);
	_ourTa1->setAttack(300);
	_enemyTa1->setAttack(300);
	_enemyTa2->setAttack(300);
	
	_ourTa2->setMinAttackInterval(1.5);
	_ourTa1->setMinAttackInterval(1.5);
	_enemyTa1->setMinAttackInterval(1.5);
	_enemyTa2->setMinAttackInterval(1.5);
	//创建亚瑟！！！
	_hero = MovingActor::create("YSdown.png", ECamp::BLUE);
	_hero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_hero->setScale(0.4);
	_hero->setTypeName("YS");
	_hero->setMinAttackInterval(0.5);
	_MapLayer->addChild(_hero);
	//测试用
	_hero->setAttackRadius(500);
	_hero->setAttack(300);
	_hero->setTag(TAG_HOUYI);
	//
	//创建鼠标监听器
	listenerMouse = EventListenerMouse::create();
	//绑定监听事件
	listenerMouse->onMouseDown = CC_CALLBACK_1(GameScene::onMouseKey, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMouse, this);
	//创建键盘监听器
	listenerKeyBoard = EventListenerKeyboard::create();
	//绑定监听事件
	listenerKeyBoard->onKeyPressed = CC_CALLBACK_2(GameScene::onPressKey, this);
	listenerKeyBoard->onKeyReleased = CC_CALLBACK_2(GameScene::onReleaseKey, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeyBoard, this);
	//创建碰撞监听器
	_listenerContact = EventListenerPhysicsContact::create();
	//绑定监听事件
	_listenerContact->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerContact, this);
	//添加摇杆视觉效果
	auto testHRocker = HRocker::createHRocker("rocker.png", "rockerBG.png", Vec2(100, 100));
	testHRocker->startRocker(false);
	this->addChild(testHRocker);
	//添加定时器
	this->scheduleUpdate();
	
	return true;
}

//碰撞检测
bool GameScene::onContactBegin(const PhysicsContact & contact)
{
	//TODO:改写成cast
	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
	Sprite* becontact;
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();
	if (tagA >=TAG_JINENG&& tagB>=TAG_JINENG)
	{
		//只处理数据
		//TODO:图片

	}
	else if (tagA >= TAG_JINENG && (tagB < TAG_JINENG&&tagB >= TAG_HERO))
	{

	}
	else if (tagA >= TAG_JINENG && (tagB < TAG_HERO&&tagB >= TAG_MOVINGACTOR))
	{

	}
	else
	{

	}
	return true;
}

//逐帧更新
void GameScene::update(float delta)
{
	//获取游戏时间
	int nowTime = GetCurrentTime()/1000;
	if (!_isover)
	{
		Node::update(delta);
		if (_beginTime == 0)
		{
			_beginTime = GetCurrentTime()/1000;
		}
		if (_enemyTa1->getLastAttackTime() == 0)
		{
			_hero->setLastAttackTime(GetCurrentTime()/1000);
		}

		//通过键位状态更新英雄移动状态
		auto leftKey = EventKeyboard::KeyCode::KEY_A; 
		auto rightKey = EventKeyboard::KeyCode::KEY_D;
		auto upKey = EventKeyboard::KeyCode::KEY_W; 
		auto downKey = EventKeyboard::KeyCode::KEY_S;
		if (isKeyPressed(leftKey))
		{
			if (isKeyPressed(upKey))
			{
				log("hereKOKO");
				keyPressedDuration(leftKey, upKey);
			}
			else if (isKeyPressed(downKey))
			{
				keyPressedDuration(leftKey, downKey);
			}
			else
			{
				keyPressedDuration(leftKey);
			}
		}
		else if (isKeyPressed(rightKey))
		{
			if (isKeyPressed(upKey))
			{
				keyPressedDuration(rightKey, upKey);
			}
			else if (isKeyPressed(downKey))
			{
				keyPressedDuration(rightKey, downKey);
			}
			else
			{
				keyPressedDuration(rightKey);
			}
		}
		else
		{
			if (isKeyPressed(upKey))
			{
				keyPressedDuration(upKey);
			}
			else if (isKeyPressed(downKey))
			{
				keyPressedDuration(downKey);
			}
		}
		if (!isKeyPressed(leftKey) && !isKeyPressed(rightKey) && isKeyPressed(upKey) && isKeyPressed(downKey))
		{
			_hero->stopAllActions();
		}
		
		//更新敌方防御塔攻击状态
		AITa(_enemyTa1);
		AITa(_enemyTa2);

		//更新小兵状态
		for (int i=0;i<4;i++)
		{
			soldierAI(ECamp::BLUE, _enemySoldiers.at(i));
			soldierMove(ECamp::BLUE, _enemySoldiers.at(i));

		}

		//更新飞行物状态
		for (auto iter = _projectiles.begin(); iter != _projectiles.end();)
		{
			if ((*iter)->calculateDistance() < 20)
			{
				_MapLayer->removeChild(*iter);
				iter = _projectiles.erase(iter);
			}
			else
			{
				(*iter)->calculatePosition();
				++iter;
			}
		}
	}
}

//鼠标按下时的回调函数
bool GameScene::onMouseKey(Event * event)
{
	auto nowTime = GetCurrentTime() / 1000;
	if (_hero->getLastAttackTime() == 0)
	{
		_hero->setLastAttackTime(nowTime);
	}
	if (nowTime - _hero->getLastAttackTime() > _hero->getMinAttackInterval())
	{
		GameScene::updateHeroTarget();
		auto tag = this->_hero->getTag();
		auto target = _hero->getAttackTarget();
		if (target)
		{
			if (tag == TAG_HOUYI)
			{
				auto projectile = Projectile::create(_hero->getAttack(), SPEED_FLY, _hero, target);
				_projectiles.pushBack(projectile);
				projectile->setTexture("arrowHY.png");
				_MapLayer->addChild(projectile);
				target->getHealth()->changeHealth(-_hero->getAttack());
				target->setLastAttackFrom(_hero);
				
			}
			else if (tag == TAG_DAJI)
			{

			}
			else if (tag == TAG_YASE)
			{

			}
			else
			{

			}
			_hero->setLastAttackTime(nowTime);
		}
	}

	
	return true;
}

//键盘按下时的回调函数
bool GameScene::onPressKey(EventKeyboard::KeyCode keyCode, Event * event)
{
	keys[keyCode] = true;
	return true;
}

//键盘抬起时的回调函数
bool GameScene::onReleaseKey(EventKeyboard::KeyCode keyCode, Event * event)
{
	keys[keyCode] = false;

	return true;
}

//判断是否一直被按住
bool GameScene::isKeyPressed(EventKeyboard::KeyCode keyCode)
{
	if (keys[keyCode])
	{
		return true;
	}
	else
	{
		return false;
	}
}

//英雄进行的操作,重载函数
void GameScene::keyPressedDuration(EventKeyboard::KeyCode code) //精灵进行的操作
{
	int offsetX = 0, offsetY = 0;
	switch (code) {
	case EventKeyboard::KeyCode::KEY_A:
		offsetX = _hero->getMoveSpeed();
		break;
	case EventKeyboard::KeyCode::KEY_D:
		offsetX = -_hero->getMoveSpeed();
		break;
	case EventKeyboard::KeyCode::KEY_W:
		offsetY = -_hero->getMoveSpeed();
		break;
	case EventKeyboard::KeyCode::KEY_S:
		offsetY = _hero->getMoveSpeed();
		break;
	default:
		offsetY = offsetX = 0;
		break;
	}
	if (_hero->getMoveSpeed() <= 0) 
	{
		offsetY = offsetX = 0;
	}
	//根据玩家朝向重置纹理图片
	std::string imgName = _hero->getTypeName();
	if (offsetX < 0 && offsetY == 0)
	{
		imgName.append(StringUtils::format("right.png"));
	}
	else if (offsetX > 0 && offsetY == 0)
	{
		imgName.append(StringUtils::format("left.png"));
	}
	else if (offsetX == 0 && offsetY < 0)
	{
		imgName.append(StringUtils::format("up.png"));
	}
	else
	{
		imgName.append(StringUtils::format("down.png"));
	}
	_hero->setTexture(imgName);
	//创建平移运动
	auto moveTo = MoveTo::create(0, Vec2(_MapLayer->getPosition().x + offsetX, _MapLayer->getPosition().y + offsetY));
	auto moveTo2 = MoveTo::create(0, Vec2(_hero->getPosition().x - offsetX, _hero->getPosition().y - offsetY));
	_MapLayer->runAction(moveTo);
	_hero->runAction(moveTo2);
}
void GameScene::keyPressedDuration(EventKeyboard::KeyCode code, EventKeyboard::KeyCode code2)
{
	int offsetX = 0, offsetY = 0;

	switch (code) {
	case EventKeyboard::KeyCode::KEY_A:
		offsetX = _hero->getMoveSpeed();
		break;
	case EventKeyboard::KeyCode::KEY_D:
		offsetX = -_hero->getMoveSpeed();
		break;
	case EventKeyboard::KeyCode::KEY_W:
		offsetY = -_hero->getMoveSpeed();
		break;
	case EventKeyboard::KeyCode::KEY_S:
		offsetY = _hero->getMoveSpeed();
		break;
	default:
		break;
	}

	switch (code2) {
	case EventKeyboard::KeyCode::KEY_A:
		offsetX = _hero->getMoveSpeed();
		break;
	case EventKeyboard::KeyCode::KEY_D:
		offsetX = -_hero->getMoveSpeed();
		break;
	case EventKeyboard::KeyCode::KEY_W:
		offsetY = -_hero->getMoveSpeed();
		break;
	case EventKeyboard::KeyCode::KEY_S:
		offsetY = _hero->getMoveSpeed();
		break;
	default:
		break;
	}
	if (_hero->getMoveSpeed() <= 0)
	{
		offsetY = offsetX = 0;
	}
	//根据玩家朝向重置纹理图片
	std::string imgName = _hero->getTypeName();
	if (offsetX < 0 && offsetY < 0)
	{
		imgName.append(StringUtils::format("upRight.png"));
	}
	else if (offsetX < 0 && offsetY > 0)
	{
		imgName.append(StringUtils::format("downRight.png"));
	}
	else if (offsetX > 0 && offsetY < 0)
	{
		imgName.append(StringUtils::format("upLeft.png"));
	}
	else if (offsetX > 0 && offsetY > 0)
	{
		imgName.append(StringUtils::format("downLeft.png"));
	}
	else if (offsetX < 0 && offsetY == 0)
	{
		imgName.append(StringUtils::format("right.png"));
	}
	else if (offsetX > 0 && offsetY == 0)
	{
		imgName.append(StringUtils::format("left.png"));
	}
	else if (offsetX == 0 && offsetY < 0)
	{
		imgName.append(StringUtils::format("up.png"));
	}
	else
	{
		imgName.append(StringUtils::format("down.png"));
	}
	_hero->setTexture(imgName);
	auto moveTo = MoveTo::create(0, Vec2(_MapLayer->getPosition().x + offsetX, _MapLayer->getPosition().y + offsetY));
	auto moveTo2 = MoveTo::create(0, Vec2(_hero->getPosition().x - offsetX, _hero->getPosition().y - offsetY));
	_MapLayer->runAction(moveTo);
	_hero->runAction(moveTo2);
}

//更新英雄的攻击目标
void GameScene::updateHeroTarget()
{
	auto delta = _enemyTa1->getPosition() - _hero->getPosition();
	auto dis = delta.length();
	if (dis < _hero->getAttackRadius())
	{
		_hero->setAttackTarget(_enemyTa1);
	}
	else 
	{
		for (int i = 0; i < _enemySoldiers.size(); i++)
		{
			delta = _enemySoldiers.at(i)->getPosition() - _hero->getPosition();
			dis = delta.length();
			if (dis < _hero->getAttackRadius())
			{
				_hero->setAttackTarget(_enemySoldiers.at(i));
			}
		}
	}
}

//调用敌方防御塔的AI行为
void GameScene::AITa(Actor* ta)
{
	//更新防御塔关于攻击的数据，并且做出攻击行为
	int nowTime = GetCurrentTime() / 1000;
	if (ta->getLastAttackTime() == 0)
	{
		ta->setLastAttackTime(nowTime);
	}
	if (nowTime - ta->getLastAttackTime() > ta->getMinAttackInterval())
	{
		//获取主体
		auto tx = ta->getPosition().x;
		auto ty = ta->getPosition().y;
		auto target = ta->getAttackTarget();
		//已经存在攻击
		if (target)
		{
			auto dis = sqrt((target->getPosition().x - tx)*(target->getPosition().x - tx) + (target->getPosition().y - ty)*(target->getPosition().y - ty));
			//超出攻击范围
			if (dis > ta->getAttackRadius())
			{
				ta->setAttackTarget(NULL);
				//按顺序遍历
				auto hx = _hero->getPosition().x;
				auto hy = _hero->getPosition().y;
				auto dis = sqrt((hx - tx)*(hx - tx) + (hy - ty)*(hy - ty));
				if (dis < ta->getAttackRadius())
				{
					ta->setAttackTarget(_hero);
				}
				else
				{

				}
			}
			else
			{
				//TODO:高仇恨吸引导致target改变
			}
		}
		//未攻击过
		else
		{
			//按顺序遍历
			auto hx = _hero->getPosition().x;
			auto hy = _hero->getPosition().y;
			auto dis = sqrt((hx - tx)*(hx - tx) + (hy - ty)*(hy - ty));
			if (dis < ta->getAttackRadius())
			{
				ta->setAttackTarget(_hero);
			}
			else
			{

			}
		}
		if (target)
		{
			auto projectile = Projectile::create(ta->getAttack(), SPEED_FLY, ta, target);
			_projectiles.pushBack(projectile);
			projectile->setTexture("arrowHY.png");
			_MapLayer->addChild(projectile);
			target->getHealth()->changeHealth(-ta->getAttack());
			target->setLastAttackFrom(ta);

			ta->setLastAttackTime(nowTime);
		}
	}
}

//小战士移动操作
void GameScene::soldierMove(ECamp ecamp, MovingActor* soldier)
{
	auto target = soldier->getAttackTarget();
	Point posTarget;
	//如果有攻击目标，那么朝向攻击目标移动
	if (target)
	{
		//如果在仇恨半径内
		auto delta = target->getPosition() - soldier->getPosition();
		auto dis = delta.length();
		if (dis < soldier->getHostilityRadius())
		{
			posTarget = soldier->getAttackTarget()->getPosition();
		}
		//不在仇恨半径内
		else
		{
			if (ecamp == ECamp::BLUE)
			{
				posTarget = _ourBirthPlace;
			}
			else
			{
				posTarget = _enemyBirthPlace;
			}
		}

	}
	//如果没有攻击目标那么朝向默认目标移动
	else
	{
		if (ecamp == ECamp::BLUE)
		{
			posTarget = _ourBirthPlace;
		}
		else
		{
			posTarget = _enemyBirthPlace;
		}
	}
	auto delta = posTarget - soldier->getPosition();
	auto distance = delta.length();
	auto dx = delta.x;
	auto dy = delta.y;
	auto speed = soldier->getMoveSpeed() / 100;
	soldier->setPosition(soldier->getPosition() + Vec2(dx / distance * speed, dy / distance * speed));
}

//小战士攻击AI
void GameScene::soldierAI(ECamp, MovingActor * soldier)
{
	bool inRadius = false;
	auto nowTime = GetCurrentTime() / 1000;
	if (soldier->getLastAttackTime() == NULL)
	{
		soldier->setLastAttackTime(nowTime);
	}
	if (nowTime - soldier->getLastAttackTime() > soldier->getMinAttackInterval())
	{
		//优先判断 如果被攻击那么攻击目标更新为攻击源头
		if (soldier->getLastAttackFrom())
		{
			auto delta = soldier->getPosition() - soldier->getLastAttackFrom()->getPosition();
			auto dis = delta.length();
			//在仇恨半径内，才会产生攻击倾向
			if (dis < soldier->getHostilityRadius())
			{
				soldier->setAttackTarget(soldier->getLastAttackFrom());
			}
			else
			{
				soldier->setLastAttackFrom(NULL);
			}
		}
		auto target = soldier->getAttackTarget();

		//如果有攻击对象，并且在攻击目标的攻击距离内,保持攻击
		if (target)
		{
			auto delta = target->getPosition() - soldier->getPosition();
			auto dis = delta.length();
			if (dis < soldier->getAttackRadius())
			{
				soldier->setAttackTarget(target);
				inRadius = true;
			}
			else
			{
				//有攻击目标，但是攻击目标不在攻击半径内，不攻击，不更新
			}
		}
		else
		{
			//在攻击范围内查询是否有可以攻击的对象


		}
		//更新完攻击对象后，如果有攻击对象，产生飞行物
		if (target&&inRadius)
		{
			auto projectile = Projectile::create(soldier->getAttack(), SPEED_FLY, soldier, target);
			_projectiles.pushBack(projectile);
			projectile->setTexture("arrowHY.png");
			_MapLayer->addChild(projectile);
			target->getHealth()->changeHealth(-soldier->getAttack());
			target->setLastAttackFrom(soldier);

			soldier->setLastAttackTime(nowTime);
		}
	}
}

