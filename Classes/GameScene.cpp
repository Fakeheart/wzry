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
	//��ʼ����
	_CharacterLayer = Layer::create();
	_MenuLayer = Layer::create();
	//������ť
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
	//��ʼ����ͼ
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
	//�����˵�
	auto menu = Menu::create(closeItem, NULL, 2);
	menu->setPosition(Vec2::ZERO);
	closeItem->setPosition(Vec2(visibleSize.width-closeItem->getContentSize().width/2, closeItem->getContentSize().height / 2));
	this->addChild(menu, 1);
	// ��ʼ���з�С��������
	_enemyBirthPlace = Vec2(5500, MAPHEIGHT / 2);
	//��ʼ���з�С��
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
	//��ʼ����ͼ�����
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
	//������ɪ������
	_hero = MovingActor::create("YSdown.png", ECamp::BLUE);
	_hero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_hero->setScale(0.4);
	_hero->setTypeName("YS");
	_hero->setMinAttackInterval(0.5);
	_MapLayer->addChild(_hero);
	//������
	_hero->setAttackRadius(500);
	_hero->setAttack(300);
	_hero->setTag(TAG_HOUYI);
	//
	//������������
	listenerMouse = EventListenerMouse::create();
	//�󶨼����¼�
	listenerMouse->onMouseDown = CC_CALLBACK_1(GameScene::onMouseKey, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerMouse, this);
	//�������̼�����
	listenerKeyBoard = EventListenerKeyboard::create();
	//�󶨼����¼�
	listenerKeyBoard->onKeyPressed = CC_CALLBACK_2(GameScene::onPressKey, this);
	listenerKeyBoard->onKeyReleased = CC_CALLBACK_2(GameScene::onReleaseKey, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeyBoard, this);
	//������ײ������
	_listenerContact = EventListenerPhysicsContact::create();
	//�󶨼����¼�
	_listenerContact->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listenerContact, this);
	//���ҡ���Ӿ�Ч��
	auto testHRocker = HRocker::createHRocker("rocker.png", "rockerBG.png", Vec2(100, 100));
	testHRocker->startRocker(false);
	this->addChild(testHRocker);
	//��Ӷ�ʱ��
	this->scheduleUpdate();
	
	return true;
}

//��ײ���
bool GameScene::onContactBegin(const PhysicsContact & contact)
{
	//TODO:��д��cast
	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
	Sprite* becontact;
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();
	if (tagA >=TAG_JINENG&& tagB>=TAG_JINENG)
	{
		//ֻ��������
		//TODO:ͼƬ

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

//��֡����
void GameScene::update(float delta)
{
	//��ȡ��Ϸʱ��
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

		//ͨ����λ״̬����Ӣ���ƶ�״̬
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
		
		//���µз�����������״̬
		AITa(_enemyTa1);
		AITa(_enemyTa2);

		//����С��״̬
		for (int i=0;i<4;i++)
		{
			soldierAI(ECamp::BLUE, _enemySoldiers.at(i));
			soldierMove(ECamp::BLUE, _enemySoldiers.at(i));

		}

		//���·�����״̬
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

//��갴��ʱ�Ļص�����
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

//���̰���ʱ�Ļص�����
bool GameScene::onPressKey(EventKeyboard::KeyCode keyCode, Event * event)
{
	keys[keyCode] = true;
	return true;
}

//����̧��ʱ�Ļص�����
bool GameScene::onReleaseKey(EventKeyboard::KeyCode keyCode, Event * event)
{
	keys[keyCode] = false;

	return true;
}

//�ж��Ƿ�һֱ����ס
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

//Ӣ�۽��еĲ���,���غ���
void GameScene::keyPressedDuration(EventKeyboard::KeyCode code) //������еĲ���
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
	//������ҳ�����������ͼƬ
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
	//����ƽ���˶�
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
	//������ҳ�����������ͼƬ
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

//����Ӣ�۵Ĺ���Ŀ��
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

//���õз���������AI��Ϊ
void GameScene::AITa(Actor* ta)
{
	//���·��������ڹ��������ݣ���������������Ϊ
	int nowTime = GetCurrentTime() / 1000;
	if (ta->getLastAttackTime() == 0)
	{
		ta->setLastAttackTime(nowTime);
	}
	if (nowTime - ta->getLastAttackTime() > ta->getMinAttackInterval())
	{
		//��ȡ����
		auto tx = ta->getPosition().x;
		auto ty = ta->getPosition().y;
		auto target = ta->getAttackTarget();
		//�Ѿ����ڹ���
		if (target)
		{
			auto dis = sqrt((target->getPosition().x - tx)*(target->getPosition().x - tx) + (target->getPosition().y - ty)*(target->getPosition().y - ty));
			//����������Χ
			if (dis > ta->getAttackRadius())
			{
				ta->setAttackTarget(NULL);
				//��˳�����
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
				//TODO:�߳����������target�ı�
			}
		}
		//δ������
		else
		{
			//��˳�����
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

//Сսʿ�ƶ�����
void GameScene::soldierMove(ECamp ecamp, MovingActor* soldier)
{
	auto target = soldier->getAttackTarget();
	Point posTarget;
	//����й���Ŀ�꣬��ô���򹥻�Ŀ���ƶ�
	if (target)
	{
		//����ڳ�ް뾶��
		auto delta = target->getPosition() - soldier->getPosition();
		auto dis = delta.length();
		if (dis < soldier->getHostilityRadius())
		{
			posTarget = soldier->getAttackTarget()->getPosition();
		}
		//���ڳ�ް뾶��
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
	//���û�й���Ŀ����ô����Ĭ��Ŀ���ƶ�
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

//Сսʿ����AI
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
		//�����ж� �����������ô����Ŀ�����Ϊ����Դͷ
		if (soldier->getLastAttackFrom())
		{
			auto delta = soldier->getPosition() - soldier->getLastAttackFrom()->getPosition();
			auto dis = delta.length();
			//�ڳ�ް뾶�ڣ��Ż������������
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

		//����й������󣬲����ڹ���Ŀ��Ĺ���������,���ֹ���
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
				//�й���Ŀ�꣬���ǹ���Ŀ�겻�ڹ����뾶�ڣ���������������
			}
		}
		else
		{
			//�ڹ�����Χ�ڲ�ѯ�Ƿ��п��Թ����Ķ���


		}
		//�����깥�����������й������󣬲���������
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

