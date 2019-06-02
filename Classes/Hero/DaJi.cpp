#include "DaJi.h"
#include "Scene/HelloWorldScene.h"
#include"Actor/BuffProjectile.h"

DaJi* DaJi::create(HelloWorld* combatScene, ECamp camp, std::string heroName, EAttackMode attackMode)
{
	DaJi* daJi = new(std::nothrow) DaJi;
	if (daJi&&daJi->init(combatScene, camp, heroName, attackMode))
	{
		daJi->autorelease();
		return daJi;
	}
	CC_SAFE_DELETE(daJi);
	return nullptr;
}

bool DaJi::init(HelloWorld* combatScene, ECamp camp, std::string heroName, EAttackMode attackMode)
{
	if (!Hero::init(combatScene, camp, heroName, attackMode))
	{
		return false;
	}
	_magicAttack = _attack;
	return true;
	
}

void DaJi::updateSprSkill_1_Position(float delta)
{
	float effectRadius = 100.f;
	auto sprSkill = dynamic_cast<Sprite*>(_combatScene->getMap()->getChildByTag(TAG_DAJI_SKILL1));
	auto deltaPosition = MyMath::calculatePositionDelta(_shockWaveAngle, SPEED_FLY * 5);
	auto newPosition = sprSkill->getPosition() + deltaPosition;
	sprSkill->setPosition(newPosition);
	float contactLimit = 20.f + sprSkill->getBoundingBox().size.width / 2;

	if (sprSkill->getPosition().distance(_skillPosition) > 400.f)
	{
		unschedule(schedule_selector(DaJi::updateSprSkill_1_Position));
		_combatScene->getMap()->removeChild(sprSkill);
		return;
	}
	auto targetPosition = Vec2::ZERO;
	auto damage = 360 + 160 * _skillLevel_1 + 1.22*_magicAttack;
	auto passiveBuff = Buff::create(EBuffType::NORMAL, 3, 0, 0, -60, 0, 0, 0, 0, 0, 0);
	for (auto it = _combatScene->_soldiers.begin(); it != _combatScene->_soldiers.end(); it++)
	{
		if (_camp != (*it)->getCamp() && (*it)->getPosition().distance(sprSkill->getPosition()) < contactLimit)
		{
			(*it)->takeBuff(passiveBuff);
			(*it)->takeDamage(EDamageType::MAGIC_DAMAGE, damage, this);
		}
	}
}

void DaJi::castSkill_1(Point mousePosition)
{
	//��������
	_magicComp->changeStateBy(-1 * _magicConsume_1);
	//���¼����ͷ�ʱ��
	auto nowTime = GetCurrentTime() / 1000.f;
	_lastSkillTime_1 = nowTime;
	//��ʼ�����ܾ���
	_shockWaveAngle = MyMath::getRad(getPosition(), mousePosition - _combatScene->getMap()->getPosition());
	_skillPosition = getPosition();
	auto sprSkill_1 = Sprite::create("pictures//others//YaSeleft.png");
	sprSkill_1->setPosition(getPosition());
	sprSkill_1->setTag(TAG_DAJI_SKILL1);
	//��֡���¼��ܾ���
	schedule(schedule_selector(DaJi::updateSprSkill_1_Position), 1 / 60, -1, 0.f);
}

void DaJi::castSkill_2()
{
	updateAttackTarget();
	if (_attackTarget)
	{
		//��������
		_magicComp->changeStateBy(-1 * _magicConsume_2);
		//���¼����ͷ�ʱ��
		auto nowTime = GetCurrentTime() / 1000.f;
		_lastSkillTime_2 = nowTime;
		//��ʼ�������˺���Ч��
		auto damage = 197.5 + 87.5*_skillLevel_2 + 0.66*_magicAttack;
		auto buff = Buff::create(EBuffType::VERTIGO, 1.5, 0, 0, -60, 0, 0, 0, 0, 0, 0);
		//��ʼ�����ܾ���
		auto sprSkill_2 = BuffProjectile::create(buff, "pictures//others//YaSeleft.png", damage, SPEED_FLY, this, _attackTarget);
		sprSkill_2->setScale(0.4);
		_combatScene->getMap()->addChild(sprSkill_2);
		sprSkill_2->setTag(TAG_DAJI_SKILL2);
		sprSkill_2->setPosition(getPosition());
	}
}

void DaJi::castSkill_3()
{
	updateAttackTarget();
	if (_attackTarget)
	{		
		//��������
		_magicComp->changeStateBy(-1 * _magicConsume_3);
		//���¼����ͷ�ʱ��
		auto nowTime = GetCurrentTime() / 1000.f;
		_lastSkillTime_3 = nowTime;
		//��Ӷ��Զ���չ�������
		for (int i = 0; i < 10; i++)
		{
			auto projecctile = Projectile::create("pictures/others/RemoteSoldierArrow.png", _magicAttack*0.5, SPEED_FLY, this, _attackTarget);
			projecctile->setScale(0.9);
			_combatScene->_readyToLaunch.insert(nowTime + 0.1*i, projecctile);
		}
	}
}
bool DaJi::attack()
{
	auto nowTime = GetCurrentTime() / 1000.f;
	//�������㹥�������ʱ�򣬰���������attack�����
	if (_isAttacking&&nowTime - _lastAttackTime < _minAttackInterval)
	{
		return false;
	}
	//���������ɹ���Ŀ��
	updateAttackTarget();
	if (!_attackTarget)
	{
		auto animation = Animation::create();
		animation = AnimationCache::getInstance()->getAnimation(StringUtils::format("%sAttackRight", _heroName.getCString()));
		animation->setDelayPerUnit(_minAttackInterval * 10);
		animation->setLoops(1);
		auto animate = Animate::create(animation);
		runAction(animate);
		return false;
	}
	//���¹���״̬��ֹͣ����
	stopAllActions();
	_isAttacking = true;
	//�����µĹ���Ŀ����㹥��ʱ����ĽǶ�
	_standingAngle = MyMath::getRad(getPosition(), _attackTarget->getPosition());
	updateDirection();
	//���Ź�������
	playAttackAnimation();
	auto projecctile = Projectile::create("pictures/others/RemoteSoldierArrow.png", _magicAttack, SPEED_FLY, this, _attackTarget);
	projecctile->setScale(0.9);
	_combatScene->_readyToLaunch.insert(nowTime + _minAttackInterval / 10 * 3, projecctile);

	return true;
}

