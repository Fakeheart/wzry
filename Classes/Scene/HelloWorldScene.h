#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Actor/Projectile.h"
#include "Actor/Soldier.h"
#include "Actor/Hero.h"
#include "Actor/Actor.h"
#include "Const/Constant.h"
#include "Const/MapInfo.h"

USING_NS_CC;

class Record;
class HRocker;
class SoldierPath;

class HelloWorld : public cocos2d::Scene
{
	CC_SYNTHESIZE(Record*, _labelRecord, LabelRecord);
	CC_SYNTHESIZE(Hero*, _myHero, MyHero);
	CC_SYNTHESIZE(Size, _visibleSize, VisibleSize);
	CC_SYNTHESIZE(Vec2, _origin, Origin);
	CC_SYNTHESIZE(HRocker*, _rocker, Rocker);
	CC_SYNTHESIZE(TMXTiledMap*, _map, Map);
	CC_SYNTHESIZE(MapInfo, _mapInformation, MapInformation);

public:
	//��������
	Vector<Ref*> _actors;
	Vector<Actor*> _towers;
	Vector<Projectile*> _bullets;
	Vector<Hero*> _heroes;
	Vector<Soldier*> _soldiers;
	Map<float, Projectile*> _readyToLaunch;
	std::vector<Damage> _damages;
private:
	SoldierPath* _soldierPathPoints;
	void generateSoldiers(float delta);
	void selectSpriteForTouch(Point touchLocation);
	//��ʼ��
	void initMapLayer();
	void initLabelRecord();
	void initHero();
	void initHRocker();
	void initSkillPanel();
	void initTower();
	void initListener();
	//֡����
	virtual void update(float delta);
	void TowerAttack();
	void updateHeroPosition();
	void updateBullets();
	void updateDamages();
	void updateSoldiersState();
	void clearObjects();
	//����
	//������
	EventListenerTouchOneByOne* listenerTouch;
	EventListenerKeyboard* listenerKeyBoard;
	//������¼�
	virtual bool onTouchBegan(Touch* touch, Event* event);
	//�����¼�
	virtual bool onPressKey(EventKeyboard::KeyCode keyCode, Event* event);
	virtual bool onReleaseKey(EventKeyboard::KeyCode keyCode, Event* event);
	//�ж��Ƿ�һֱ����ס
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	//��������
	bool _isMouseSprite;
	Sprite* _mouseSprite;
	EventListenerMouse* listenerMouse;
	virtual bool onMouseMove(Event* event);
	void updateSkillPanel();

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);
};

#endif 
