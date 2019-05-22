#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Component/StateComponent.h"
#include "GameController/HRocker.h"
#include "Actor/Actor.h"
#include "../Component/Record.h"
#include "Component/ExpComponent.h"
#include "Actor/Projectile.h"
#include "Actor/Hero.h"
#include "GameController/HRocker.h"


USING_NS_CC;


Scene* GameScene::createScene()
{
    return GameScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");
}

bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
	
	_visibleSize = Director::getInstance()->getVisibleSize();
	_origin = Director::getInstance()->getVisibleOrigin();

	initMapLayer();
	initHero();
	initLabelRecord();
	initHRocker();
	loadingAnimation();

	scheduleUpdate();
//	test();
//	schedule(schedule_selector(GameScene::generateSoldiers), 30.f, -1, 0.f);
	

    return true;
}

void GameScene::generateSoldiers(float delta)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto soldier = Soldier::create(EAttackMode::MELEE, ECamp::BLUE, ERoad::DOWNWAY);
	soldier->setPosition(visibleSize / 4);
	soldier->setScale(0.1);
	soldier->setFirstDest(visibleSize);
	soldier->setSecondDest(visibleSize);
	_map->addChild(soldier);
	_soldiers.pushBack(soldier);
}

void GameScene::initMapLayer()
{
	_map = TMXTiledMap::create("map/map1.tmx");
	auto size = _map->getBoundingBox().size;
	_map->setAnchorPoint(Vec2::ZERO);
	log("%f", size.height);
	_map->setPosition(Vec2::ZERO);
//	_map->setScale(2);

	auto layerTower = _map->getLayer("Tower");
	layerTower->setVisible(false);

	addChild(_map, 0, TAG_MAP);
}

void GameScene::initLabelRecord()
{
	_labelRecord = Record::create();
	addChild(_labelRecord);
}

void GameScene::initHero()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	_myHero = Hero::create(ECamp::BLUE, "HouYi", EAttackMode::MELEE);
	_myHero->setPosition(visibleSize / 2);
	_myHero->setTag(TAG_MYHERO);
	_myHero->setScale(0.5);
	_myHero->setRecordComp(_labelRecord);
	_map->addChild(_myHero);
	_actors.pushBack(_myHero);
}

void GameScene::initHRocker()
{
	_rocker = HRocker::createHRocker("rocker.png", "rockerBG.png", Vec2(200, 100));
	_rocker->startRocker(true);
	addChild(_rocker);
}

void GameScene::loadingAnimation()
{
	String heroName = "HouYi";

	auto animation_00 = Animation::create();
	animation_00->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sup1.png", heroName.getCString(), heroName.getCString()));
	animation_00->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sup2.png", heroName.getCString(), heroName.getCString()));
	AnimationCache::getInstance()->addAnimation(animation_00,StringUtils::format("%sMoveUp",heroName.getCString()));

	auto animation_01 = Animation::create();
	animation_01->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%supLeft1.png", heroName.getCString(), heroName.getCString()));
	animation_01->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%supLeft2.png", heroName.getCString(), heroName.getCString()));
	AnimationCache::getInstance()->addAnimation(animation_01, StringUtils::format("%sMoveUpLeft", heroName.getCString()));

	auto animation_02 = Animation::create();
	animation_02->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%supRight1.png", heroName.getCString(), heroName.getCString()));
	animation_02->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%supRight2.png", heroName.getCString(), heroName.getCString()));
	AnimationCache::getInstance()->addAnimation(animation_02, StringUtils::format("%sMoveUpRight", heroName.getCString()));

	auto animation_03 = Animation::create();
	animation_03->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sleft1.png", heroName.getCString(), heroName.getCString()));
	animation_03->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sleft2.png", heroName.getCString(), heroName.getCString()));
	AnimationCache::getInstance()->addAnimation(animation_03, StringUtils::format("%sMoveLeft", heroName.getCString()));

	auto animation_04 = Animation::create();
	animation_04->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sright1.png", heroName.getCString(), heroName.getCString()));
	animation_04->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sright2.png", heroName.getCString(), heroName.getCString()));
	AnimationCache::getInstance()->addAnimation(animation_04, StringUtils::format("%sMoveRight", heroName.getCString()));

	auto animation_05 = Animation::create();
	animation_05->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sdown1.png", heroName.getCString(), heroName.getCString()));
	animation_05->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sdown2.png", heroName.getCString(), heroName.getCString()));
	AnimationCache::getInstance()->addAnimation(animation_05, StringUtils::format("%sMoveDown", heroName.getCString()));

	auto animation_06 = Animation::create();
	animation_06->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sdownLeft1.png", heroName.getCString(), heroName.getCString()));
	animation_06->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sdownLeft2.png", heroName.getCString(), heroName.getCString()));
	AnimationCache::getInstance()->addAnimation(animation_06, StringUtils::format("%sMoveDownLeft", heroName.getCString()));

	auto animation_07 = Animation::create();
	animation_07->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sdownRight1.png", heroName.getCString(), heroName.getCString()));
	animation_07->addSpriteFrameWithFileName(StringUtils::format("pictures\\hero\\%s\\%sdownRight2.png", heroName.getCString(), heroName.getCString()));
	AnimationCache::getInstance()->addAnimation(animation_07, StringUtils::format("%sMoveDownRight", heroName.getCString()));
}

void GameScene::update(float delta)
{
	updateHeroPosition();
	for (auto it=_bullets.begin();it!=_bullets.end();)
	{
		if (!(*it)->getTarget()->getAlreadyDead())
		{
			if ((*it)->calculateDistance() < (*it)->getTarget()->getBoundingBox().size.height / 2)
			{
				auto target = (*it)->getTarget();
				target->takeDamage((*it)->getDamage(), (*it)->getFromActor());
				removeChild(*it);
				it = _bullets.erase(it);
			}
			else
			{
				(*it)->calculatePosition();
				++it;
			}
		}
		else
		{
			removeChild(*it);
			it = _bullets.erase(it);
		}
	}

	for (auto it = _actors.begin(); it !=_actors.end(); ++it)
	{
		if ((*it)->getAlreadyDead())
		{
			(*it)->setVisible(false);
		}
	}
}

void GameScene::updateHeroPosition()
{
	if (_rocker->getIsCanMove())
	{
		_myHero->setStandingAngle(_rocker->getAngle());
		_myHero->heroMove();
		auto movingSpeed = _myHero->getMoveSpeed();
		auto position = _map->getPosition();
		auto angle = _myHero->getStandingAngle();
		_map->setPosition(position - Vec2(cos(angle) * movingSpeed / 60, sin(angle) * movingSpeed / 60));
	}
	else
	{
		_myHero->stopMove();
	}
}

void GameScene::launch(float delta)
{
	auto from = dynamic_cast<Actor*>(getChildByTag(0));
	auto to = dynamic_cast<Actor*>(getChildByTag(1));
	
	if (from && !from->getAlreadyDead() && to && !to->getAlreadyDead())
	{
		auto bullet = Projectile::create(600, 10, from, to);
		addChild(bullet);
		_bullets.pushBack(bullet);
	}
}

void GameScene::test()
{
	auto animation = AnimationCache::getInstance()->getAnimation("YSMoveDownRight");
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.1f);
	auto animate = Animate::create(animation);

	_myHero->runAction(RepeatForever::create(animate));
}
