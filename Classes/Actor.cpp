#include "Actor.h"
#include "HealthComponent.h"
#include "Constant.h"
#include "Constant.h"


Actor* Actor::create(const std::string& filename,ECamp camp)
{
	Actor *sprite = new (std::nothrow)Actor();
	if (sprite&&sprite->init(filename, camp))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Actor::init(const std::string& filename,ECamp thisCamp)
{
	if (!Sprite::initWithFile(filename)) {
		return false;
	}
	_health = HealthComponent::create(2000.0, 1.0);
	this->addChild(_health);
	auto size = getBoundingBox().size;

	_health->setPosition(getPosition() + Vec2(size.width / 2, size.height * 4 / 5));
	_hostilityRadius = 500;
	_lastAttackTime = 0;
	_attack = 100;
	setAlreadyDead(false);
	setDefense(ORIGIN_DEFENSE);
	setAttack(ORIGIN_ATTACK);
	setAttackRadius(ORIGIN_RADIUS);
	setCamp(thisCamp);
	//TODO : BONUS
	setAttack(ORIGIN_INTERVAL);
	//TODO : PlayerName
	return true;
}
bool Actor::attack()
{
	return true;
}

bool Actor::onTakeDamage(float attack, Actor * fromActor)
{


	return true;
}

bool Actor::die()
{
	return true;
}
