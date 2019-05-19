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
	//按键状态
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	cocos2d::TMXTiledMap* _map;
	cocos2d::TMXLayer* _wall;
	Layer * _CharacterLayer;
	Layer * _MapLayer;
	Layer * _MenuLayer;

	static cocos2d::Scene* createScene();
	virtual bool init();
	
	static void problemLoading(const char* filename);
	//测试的玩意儿
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
	//双方出生地
	Point _enemyBirthPlace;
	Point _ourBirthPlace;
	//敌方小兵容器
	Vector<MovingActor*>_enemySoldiers;

	//飞行物容器
	Vector<Projectile*>_projectiles;

	//碰撞检测
	bool onContactBegin(const PhysicsContact& contact);

	//游戏开始时间
	int _beginTime;
	//游戏结束
	bool _isover;
	//逐帧操作
	void update(float delta) override;
	//按键事件
	virtual bool onMouseKey(Event* event);
	virtual bool onPressKey(EventKeyboard::KeyCode keyCode, Event* event);
	virtual bool onReleaseKey(EventKeyboard::KeyCode keyCode, Event* event);
	//virtual bool updateState(EventKeyboard::KeyCode keyCode, int type);
	//virtual bool updateDirection();
	//判断是否一直被按住
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);

	//精灵进行的操作
	void keyPressedDuration(EventKeyboard::KeyCode code);
	void keyPressedDuration(EventKeyboard::KeyCode code, EventKeyboard::KeyCode code2);
	//更新英雄的攻击目标
	void updateHeroTarget();
	//防御塔AI
	void AITa(Actor* ta);


	//小战士移动操作
	void soldierMove(ECamp, MovingActor* soldier);
	//小战士攻击AI
	void soldierAI(ECamp, MovingActor* soldier);

	CREATE_FUNC(GameScene);
};


#endif // !_SingleScene_Scene_H_
