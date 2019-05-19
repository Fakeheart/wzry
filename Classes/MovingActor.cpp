#include "Actor.h"
#include "HealthComponent.h"
#include "Constant.h"
#include "Constant.h"
#include "MovingActor.h"

bool MovingActor::die()
{
	return false;
}

bool MovingActor::attack()
{
	return false;
}

MovingActor* MovingActor::create(const std::string& filename, ECamp camp)
{
	MovingActor *sprite = new (std::nothrow)MovingActor();
	if (sprite&&sprite->init(filename, camp))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool MovingActor::init(const std::string& filename, ECamp thisCamp)
{
	if (!Sprite::initWithFile(filename)) {
		return false;
	}
	_hostilityRadius = 300;
	_moveSpeed = 10;
	_magicDefense = 100;
	_health = HealthComponent::create(2000.0, 1.0);
	this->addChild(_health);
	auto size = getBoundingBox().size;

	_health->setPosition(getPosition() + Vec2(size.width / 2, size.height * 4 / 5));
	//还有很多没init，自己去看.h

	setAlreadyDead(false);
	setDefense(ORIGIN_DEFENSE);
	setAttack(ORIGIN_ATTACK);
	setAttackRadius(ORIGIN_RADIUS);
	setMinAttackInterval(0.8);
	setCamp(thisCamp);
	//TODO : BONUS
	//TODO : PlayerName
	return true;
}
