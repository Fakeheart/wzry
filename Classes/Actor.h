#pragma once
#ifndef _Actor_h_

#define _Actor_h_

#include<iostream>
#include<string>
#include<time.h>
#include "cocos2d.h"
#include"Constant.h"
USING_NS_CC;

class HealthComponent;
class Bonus;
class Projectile;
class Actor: public cocos2d::Sprite 
{

	CC_SYNTHESIZE(HealthComponent*, _health, Health);
	CC_SYNTHESIZE(bool, _alreadyDead, AlreadyDead);
	CC_SYNTHESIZE(float, _defense, Defense);
	CC_SYNTHESIZE(float, _attack, Attack);
	CC_SYNTHESIZE(float, _attackRadius, AttackRadius);
	CC_SYNTHESIZE(ECamp, _camp, Camp);
	CC_SYNTHESIZE(Bonus*, _bonus, Bonus);
	CC_SYNTHESIZE(float, _minAttackInterval, MinAttackInterval);
	CC_SYNTHESIZE(std::string, _playerName, PlayerName);
	CC_SYNTHESIZE(float, _lastAttackTime, LastAttackTime);

	CC_SYNTHESIZE(Actor*, _attackTarget, AttackTarget);
	CC_SYNTHESIZE(Actor*, _lastAttackFrom, LastAttackFrom);
	CC_SYNTHESIZE(Vector<Actor*> , _assitActors, AssitActors);

	CC_SYNTHESIZE(float, _hostilityRadius, HostilityRadius);

	CC_SYNTHESIZE(std::string, _typeName, TypeName);
public:
	//±»¹¥»÷
	bool onTakeDamage(float attack, Actor* fromActor);
	
	virtual bool die();
	virtual bool attack();
	//
	static Actor* create(const std::string& filename, ECamp camp);
	virtual bool init(const std::string& filename,ECamp camp);
};
#endif // !Actor_h
