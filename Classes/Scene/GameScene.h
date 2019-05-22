#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "cocos2d.h"
#include "Actor/Projectile.h"
#include "Actor/Actor.h"
#include "Actor/Soldier.h"

USING_NS_CC;

class Record;
class Hero;
class HRocker;

class GameScene : public cocos2d::Scene
{
	CC_SYNTHESIZE(Vector<Soldier*>, _soldiers, AllSoldiers);
	CC_SYNTHESIZE(Vector<Projectile*>, _bullets, Bullets);
	CC_SYNTHESIZE(Vector<Actor*>, _actors, AllActor);
	CC_SYNTHESIZE(Record*, _labelRecord, LabelRecord);
	CC_SYNTHESIZE(Hero*, _myHero, MyHero);
	CC_SYNTHESIZE(Size, _visibleSize, VisibleSize);
	CC_SYNTHESIZE(Vec2, _origin, Origin);
	CC_SYNTHESIZE(HRocker*, _rocker, Rocker);
	CC_SYNTHESIZE(TMXTiledMap*, _map, Map);

private:

	void generateSoldiers(float delta);

	void initMapLayer();

	void initLabelRecord();

	void initHero();

	void initHRocker();

	void updateHeroPosition();

public:

    static cocos2d::Scene* createScene();

    virtual bool init();

	virtual void loadingAnimation();
	
	virtual void update(float delta);

	void launch(float delta);

	void test();

    CREATE_FUNC(GameScene);

};

#endif 
