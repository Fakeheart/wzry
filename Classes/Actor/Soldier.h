#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "cocos2d.h"
#include "MovingActor.h"
#include "Component/StateComponent.h"
#include <vector>

USING_NS_CC;
using std::vector;

class Soldier :public MovingActor
{
	CC_SYNTHESIZE(bool, _isMovingToDest, IsMovingToDest);
	CC_SYNTHESIZE(EAttackMode, _attackMode, AttackMode);
	CC_SYNTHESIZE(Vec2, _firstDest, FirstDest);
	CC_SYNTHESIZE(Vec2, _secondDest, SecondDest);
	CC_SYNTHESIZE(bool, _arrivedFirstDest, ArrivedFirstDest);
	CC_SYNTHESIZE(bool, _isDisturbed, IsDisturbed);
	CC_SYNTHESIZE(Actor*, _instigator, Instigator);
	CC_SYNTHESIZE(vector<Vec2>, _pathPoints, PathPoints);
	CC_SYNTHESIZE(Vec2, _nextDest, NextDest);

private:

	void initData(EAttackMode attackMode, ECamp camp, ERoad road);

	void initHealthComp();

public:

	void moveToNextDest();

	virtual bool init(EAttackMode attackMode, ECamp camp, ERoad road);

	static Soldier* create(EAttackMode attackMode, ECamp camp, ERoad road);
};


#endif // !__SOLDIER_H__
