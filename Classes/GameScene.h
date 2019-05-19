#pragma once

#ifndef _GameScene_Scene_H_
#define _GameScene_Scene_H_

#include "cocos2d.h"
#include "Actor.h"
#include "MovingActor.h"
#include "Projectile.h"
USING_NS_CC;

class Hero;

class GameScene :public cocos2d::Scene
{
protected:

public:
	EventListenerMouse* listenerMouse;
	EventListenerKeyboard* listenerKeyBoard;
	EventListenerPhysicsContact* _listenerContact;
	//����״̬
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	cocos2d::TMXTiledMap* _map;
	cocos2d::TMXLayer* _wall;
	Layer * _CharacterLayer;
	Layer * _MapLayer;
	Layer * _MenuLayer;

	static cocos2d::Scene* createScene();
	virtual bool init();
	
	static void problemLoading(const char* filename);
	//���Ե������
	PhysicsBody* _pOurTa1;
	PhysicsBody* _pOurTa2;
	PhysicsBody* _pEnemyTa1;
	PhysicsBody* _pEnemyTa2;

	Actor* _ourTa1;
	Actor* _ourTa2;
	Actor* _enemyTa1;
	Actor* _enemyTa2;

	PhysicsBody* _pHero;
	MovingActor* _hero;
	//˫��������
	Point _enemyBirthPlace;
	Point _ourBirthPlace;
	//�з�С������
	Vector<MovingActor*>_enemySoldiers;

	//����������
	Vector<Projectile*>_projectiles;

	//��ײ���
	bool onContactBegin(const PhysicsContact& contact);

	//��Ϸ��ʼʱ��
	int _beginTime;
	//��Ϸ����
	bool _isover;
	//��֡����
	void update(float delta) override;
	//�����¼�
	virtual bool onMouseKey(Event* event);
	virtual bool onPressKey(EventKeyboard::KeyCode keyCode, Event* event);
	virtual bool onReleaseKey(EventKeyboard::KeyCode keyCode, Event* event);
	//virtual bool updateState(EventKeyboard::KeyCode keyCode, int type);
	//virtual bool updateDirection();
	//�ж��Ƿ�һֱ����ס
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);

	//������еĲ���
	void keyPressedDuration(EventKeyboard::KeyCode code);
	void keyPressedDuration(EventKeyboard::KeyCode code, EventKeyboard::KeyCode code2);
	//����Ӣ�۵Ĺ���Ŀ��
	void updateHeroTarget();
	//������AI
	void AITa(Actor* ta);


	//Сսʿ�ƶ�����
	void soldierMove(ECamp, MovingActor* soldier);
	//Сսʿ����AI
	void soldierAI(ECamp, MovingActor* soldier);

	CREATE_FUNC(GameScene);
};


#endif // !_SingleScene_Scene_H_
